#pragma once

#include "prettyurl/app/logging/details/registry.hpp"

#include <concepts>

namespace prettyurl::app::logging {

template <typename T, typename... Args>
requires(std::is_base_of_v<core::logging::base_logger, T> == true)
inline std::shared_ptr<core::logging::base_logger> create(std::string_view name, Args&&... args) {
  const auto logger = std::make_shared<T>(name, std::forward<decltype(args)>(args)...);
  details::registry::instance().register_logger(name, logger);

  return logger;
}

[[nodiscard]] inline std::shared_ptr<core::logging::base_logger> get(std::string_view name) {
  return details::registry::instance().get(name);
}

[[nodiscard]] inline std::shared_ptr<core::logging::base_logger> get_default_logger() {
  return details::registry::instance().get_default_logger();
}

[[nodiscard]] inline core::logging::base_logger* get_default_logger_raw() {
  return details::registry::instance().get_default_logger_raw();
}

[[nodiscard]] inline core::logging::level::elevel get_log_level() noexcept {
  return get_default_logger_raw()->get_log_level();
} 

inline void set_default_logger(std::shared_ptr<core::logging::base_logger> logger) {
  return details::registry::instance().set_default_logger(std::move(logger));
}

template <typename... Args>
inline void log(const core::logging::level::elevel level, const char* format, Args&&... args) {
  return get_default_logger_raw()->log(level, std::forward<decltype(args)>(args)...);
}

template <typename... Args>
inline void trace(const char* format, Args&&... args) {
  return get_default_logger_raw()->trace(format, std::forward<decltype(args)>(args)...);
}

template <typename... Args>
inline void debug(const char* format, Args&&... args) {
  return get_default_logger_raw()->debug(format, std::forward<decltype(args)>(args)...);
}

template <typename... Args>
inline void info(const char* format, Args&&... args) {
  return get_default_logger_raw()->info(format, std::forward<decltype(args)>(args)...);
}

template <typename... Args>
inline void warn(const char* format, Args&&... args) {
  return get_default_logger_raw()->warn(format, std::forward<decltype(args)>(args)...);
}  

template <typename... Args>
inline void error(const char* format, Args&&... args) {
  return get_default_logger_raw()->error(format, std::forward<decltype(args)>(args)...);
}    

template <typename... Args>
inline void fatal(const char* format, Args&&... args) {
  return get_default_logger_raw()->fatal(format, std::forward<decltype(args)>(args)...);
}

inline void set_log_level(const core::logging::level::elevel level) noexcept {
  get_default_logger_raw()->set_log_level(level);
}

inline void release() {
  details::registry::instance().release();
}

} // namespace prettyurl::app::logging