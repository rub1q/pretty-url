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
inline void log(core::logging::source_loc loc, const core::logging::level::elevel level, const char* format, Args&&... args) {
  return get_default_logger_raw()->log(std::move(loc), level, format, std::forward<decltype(args)>(args)...);
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

inline prettyurl::core::logging::base_logger* operator""_logger(const char* name, std::size_t) {
  return prettyurl::app::logging::get(name).get();
}

#define TRC   prettyurl::core::logging::level::trace
#define DBG   prettyurl::core::logging::level::debug
#define INF   prettyurl::core::logging::level::info
#define WRN   prettyurl::core::logging::level::warning
#define ERR   prettyurl::core::logging::level::error
#define FTL   prettyurl::core::logging::level::fatal

#define PU_LOG_IMPL_TO(logger, level, format, ...) \
  if (logger) { (logger)->log(prettyurl::core::logging::source_loc::current(), level, format __VA_OPT__(, ) __VA_ARGS__); }

#define PU_LOG_TRC_TO(logger, format, ...) \
  PU_LOG_IMPL_TO(logger, TRC, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_DBG_TO(logger, format, ...) \
  PU_LOG_IMPL_TO(logger, DBG, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_INF_TO(logger, format, ...) \
  PU_LOG_IMPL_TO(logger, INF, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_WRN_TO(logger, format, ...) \
  PU_LOG_IMPL_TO(logger, WRN, format __VA_OPT__( ,) __VA_ARGS__)

#define PU_LOG_ERR_TO(logger, format, ...) \
  PU_LOG_IMPL_TO(logger, ERR, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_FTL_TO(logger, format, ...) \
  PU_LOG_IMPL_TO(logger, FTL, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_TRC(format, ...) \
  PU_LOG_IMPL_TO(prettyurl::app::logging::get_default_logger_raw(), TRC, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_DBG(format, ...) \
  PU_LOG_IMPL_TO(prettyurl::app::logging::get_default_logger_raw(), DBG, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_INF(format, ...) \
  PU_LOG_IMPL_TO(prettyurl::app::logging::get_default_logger_raw(), INF, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_WRN(format, ...) \
  PU_LOG_IMPL_TO(prettyurl::app::logging::get_default_logger_raw(), WRN, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_ERR(format, ...) \
  PU_LOG_IMPL_TO(prettyurl::app::logging::get_default_logger_raw(), ERR, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_FTL(format, ...) \
  PU_LOG_IMPL_TO(prettyurl::app::logging::get_default_logger_raw(), FTL, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG(level, format, ...) \
  PU_LOG_IMPL_TO(level, format __VA_OPT__(, ) __VA_ARGS__)

#define PU_LOG_TO(logger, level, format, ...) \
  PU_LOG_IMPL_TO(logger, level, format __VA_OPT__(, ) __VA_ARGS__)
