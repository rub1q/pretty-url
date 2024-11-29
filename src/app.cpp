#include "prettyurl/app.hpp"

#include "prettyurl/app/logging/log-inl.hpp"
#include "prettyurl/infra/logging/loggers/console_logger.hpp"
#include "prettyurl/infra/logging/loggers/file_logger.hpp"
#include "prettyurl/infra/net/http/server.hpp"

namespace prettyurl {

void application::run() {
  auto clogger = app::logging::create<infra::logging::loggers::console_logger>("console");
  auto flogger = app::logging::create<infra::logging::loggers::file_logger>("file", "./app.log");

  app::logging::set_default_logger(flogger);
  app::logging::set_log_level(core::logging::level::debug);

  clogger->info("start init app");
  flogger->info("start init app");

  infra::net::http::server([](auto&&) {
    infra::net::http::response resp;
    
    resp.status_code(core::net::http::status::ok);
    resp.body("request received\n");
    resp.prepare_payload();

    return resp;
  }).concurrency(4)
    .listen_and_serve(8080);
  
  app::logging::release();
}

} // namespace prettyurl