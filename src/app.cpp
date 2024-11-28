#include "prettyurl/app.hpp"

#include "prettyurl/app/logging/log-inl.hpp"
#include "prettyurl/infra/logging/loggers/console_logger.hpp"
#include "prettyurl/infra/logging/loggers/file_logger.hpp"

namespace prettyurl {

void application::run() {
  auto clogger = app::logging::create<infra::logging::loggers::console_logger>("console");
  auto flogger = app::logging::create<infra::logging::loggers::file_logger>("file", "./app.log");

  app::logging::set_default_logger(flogger);
  app::logging::set_log_level(core::logging::level::debug);

  clogger->info("start init app");
  flogger->info("start init app");
}

} // namespace prettyurl