#pragma once

#include "prettyurl/utils/format.hpp"
#include "prettyurl/core/logging/common.hpp" 

namespace prettyurl::core::logging {

class base_logger {
public:
  virtual ~base_logger() = default;

  constexpr explicit base_logger(std::string_view name)
    : name_(std::move(name)) {
  }

  template <typename... Args>
  void log(const level::elevel level, const char* format, Args&&... args) {
    if (log_lvl_ > level) {
      return;
    }
    
    const auto fmt_str = utils::format(format, std::forward<decltype(args)>(args)...);
    do_append(level, fmt_str);
  }

  template <typename... Args>
  void trace(const char* format, Args&&... args) {
    log(level::trace, format, std::forward<decltype(args)>(args)...);
  }

  template <typename... Args>
  void debug(const char* format, Args&&... args) {
    log(level::debug, format, std::forward<decltype(args)>(args)...);
  }

  template <typename... Args>
  void info(const char* format, Args&&... args) {
    log(level::info, format, std::forward<decltype(args)>(args)...);
  }

  template <typename... Args>
  void warn(const char* format, Args&&... args) {
    log(level::warning, format, std::forward<decltype(args)>(args)...);
  }  

  template <typename... Args>
  void error(const char* format, Args&&... args) {
    log(level::error, format, std::forward<decltype(args)>(args)...);
  }    

  template <typename... Args>
  void fatal(const char* format, Args&&... args) {
    log(level::fatal, format, std::forward<decltype(args)>(args)...);
  }   

  void set_log_level(const level::elevel level) noexcept {
    log_lvl_ = level;
  }

  [[nodiscard]] level::elevel get_log_level() const noexcept {
    return log_lvl_;
  }

  [[nodiscard]] std::string_view name() const noexcept {
    return name_;
  }  

protected:
  virtual void do_append(const level::elevel level, std::string_view text) = 0;

private:
  level::elevel log_lvl_ { level::none };
  std::string name_;
};

} // namespace prettyurl::core::logging