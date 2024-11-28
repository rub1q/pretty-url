#include "prettyurl/app/logging/details/registry.hpp"

namespace prettyurl::app::logging {
namespace details {

registry& registry::instance() noexcept {
  static registry reg{};
  return reg;
}

void registry::register_logger(std::string_view name, std::shared_ptr<core::logging::logger> logger) {
  std::unique_lock lk { mtx_ };
  loggers_[name.data()] = std::move(logger);
}

void registry::set_default_logger(std::shared_ptr<core::logging::logger> logger) {
  std::unique_lock lk { mtx_ };
  default_logger_ = std::move(logger);
}

std::shared_ptr<core::logging::logger> registry::get_default_logger() const noexcept {
  std::shared_lock lk { mtx_ };
  return default_logger_;
}

std::shared_ptr<core::logging::logger> registry::get(std::string_view name) const noexcept {
  std::shared_lock lk { mtx_ };

  if (!loggers_.contains(name.data())) {
    return default_logger_;
  }

  return loggers_.at(name.data());
}

core::logging::logger* registry::get_default_logger_raw() const noexcept {
  return default_logger_.get();
}

void registry::release() {
  std::unique_lock lk { mtx_ };

  loggers_.clear();
  default_logger_.reset(new core::logging::loggers::null_logger());
}

} // namespace details
} // namespace prettyurl::app::logging