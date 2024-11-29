#pragma once

#include "prettyurl/core/logging/base_logger.hpp"
#include "prettyurl/infra/logging/loggers/spd_logger_impl.hpp"

#include <filesystem>

namespace prettyurl::infra::logging {
namespace loggers {

class file_logger final : public core::logging::base_logger {
public:
  ~file_logger();
  
  explicit file_logger(std::string_view name, const std::filesystem::path& log_path);

  file_logger(const file_logger&) = delete;
  file_logger(file_logger&&) = delete;

  file_logger& operator=(const file_logger&) = delete;
  file_logger& operator=(file_logger&&) = delete;

private:
  void do_append(const core::logging::level::elevel level, std::string_view text) final;

private:
  std::shared_ptr<spd_logger_impl> pimpl_ { std::make_shared<spd_logger_impl>() };
};

} // namespace loggers
} // namespace prettyurl::infra::logging