#include "prettyurl/infra/logging/loggers/file_logger.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace prettyurl::infra::logging {
namespace loggers {

using namespace std::literals;

namespace fs = std::filesystem;

file_logger::~file_logger() {
  spdlog::shutdown();
}

file_logger::file_logger(std::string_view name, const fs::path& log_path) 
  : core::logging::logger(name) {
  if (!fs::exists(log_path)) {
    fs::create_directories(log_path.parent_path());
  }

  auto path = fs::weakly_canonical(log_path).string();
  
  pimpl_->logger = spdlog::basic_logger_mt<spdlog::async_factory>(name.data(), path);
  pimpl_->logger->set_level(spdlog::level::trace);

  spdlog::flush_every(3s);
}

void file_logger::do_append(const core::logging::level::elevel level, std::string_view text) {
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