#pragma once

#include "prettyurl/core/logging/logger.hpp"
#include "prettyurl/core/logging/loggers/null_logger.hpp"

#include <memory>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

namespace prettyurl::app::logging {
namespace details {

class registry final {
public:
  registry(const registry&) = delete;
  registry(registry&&) = delete;

  registry& operator=(const registry&) = delete;
  registry& operator=(registry&&) = delete;

  static registry& instance() noexcept;

  void register_logger(std::string_view name, std::shared_ptr<core::logging::logger> logger);
  void set_default_logger(std::shared_ptr<core::logging::logger> logger);
  void release();

  [[nodiscard]] std::shared_ptr<core::logging::logger> get_default_logger() const noexcept;
  [[nodiscard]] std::shared_ptr<core::logging::logger> get(std::string_view name) const noexcept;
  [[nodiscard]] core::logging::logger* get_default_logger_raw() const noexcept;

private:
  registry() = default;

private:
  mutable std::shared_mutex mtx_;
  std::shared_ptr<core::logging::logger> default_logger_ { std::make_shared<core::logging::loggers::null_logger>() };

  using loggers_tab = std::unordered_map<std::string, std::shared_ptr<core::logging::logger>>;
  loggers_tab loggers_;
};

} // namespace details
} // namespace prettyurl::app::logging