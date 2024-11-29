#pragma once

#include "prettyurl/core/logging/base_logger.hpp"
#include "prettyurl/infra/logging/loggers/spd_logger_impl.hpp"

namespace prettyurl::infra::logging {
namespace loggers {

class console_logger final : public core::logging::base_logger {
public:
  ~console_logger();
  
  explicit console_logger(std::string_view name);

private:
  void do_append(const core::logging::level::elevel level, std::string_view text) final;

private:
  std::shared_ptr<spd_logger_impl> pimpl_ { std::make_shared<spd_logger_impl>() }; 
};

} // namespace loggers
} // namespace prettyurl::infra::logging