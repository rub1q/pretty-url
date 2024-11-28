#pragma once

#include "spdlog/spdlog.h"

namespace prettyurl::infra::logging {
namespace loggers {
 
struct spd_logger_impl final {
  std::shared_ptr<spdlog::logger> logger { nullptr };
};

} // namespace loggers
} // namespace prettyurl::infra::logging