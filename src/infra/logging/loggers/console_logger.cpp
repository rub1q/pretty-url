#include "prettyurl/infra/logging/loggers/console_logger.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace prettyurl::infra::logging {
namespace loggers {

using namespace std::literals;

console_logger::~console_logger() {
  spdlog::shutdown();
}

console_logger::console_logger(std::string_view name) 
  : core::logging::logger(name) {
  pimpl_->logger = spdlog::stdout_color_mt<spdlog::async_factory>(name.data());
  pimpl_->logger->set_level(spdlog::level::trace);

  spdlog::flush_every(3s);
}

void console_logger::do_append(const core::logging::level::elevel level, std::string_view text) {
  if (!pimpl_ || !pimpl_->logger) {
    return;
  }

  switch (level) {
    case core::logging::level::trace :
      return pimpl_->logger->trace(std::move(text));
    case core::logging::level::debug :
      return pimpl_->logger->debug(std::move(text));
    case core::logging::level::info :
      return pimpl_->logger->info(std::move(text));
    case core::logging::level::warning :
      return pimpl_->logger->warn(std::move(text));
    case core::logging::level::error :
      return pimpl_->logger->error(std::move(text));
    case core::logging::level::fatal :
      return pimpl_->logger->critical(std::move(text));

    default : 
      return;
  }
}

} // namespace loggers
} // namespace prettyurl::infra::logging