#include "prettyurl/app.hpp"

#include "prettyurl/app/logging/log-inl.hpp"
#include "prettyurl/infra/logging/loggers/console_logger.hpp"
#include "prettyurl/infra/logging/loggers/file_logger.hpp"
#include "prettyurl/infra/net/http/server.hpp"
#include "prettyurl/infra/net/http/router.hpp"

namespace prettyurl {

using namespace std::literals;

void application::run() {
  auto clogger = app::logging::create<infra::logging::loggers::console_logger>("console");
  auto flogger = app::logging::create<infra::logging::loggers::file_logger>("file", "/var/log/pretty-url/app.log");

  app::logging::set_default_logger(flogger);
  app::logging::set_log_level(core::logging::level::debug);

  PU_LOG_INF("start init app");
  PU_LOG_INF_TO("console"_logger, "start init app");

  infra::net::http::router router;

  router.add_route("/hello"sv, core::net::http::emethod::get | core::net::http::emethod::post, [](auto&&) {
    infra::net::http::response resp;
    
    resp.status_code(core::net::http::estatus::ok);
    resp.body("hello!\n");
    resp.prepare_payload();

    return resp;
  });

  infra::net::http::server(std::move(router))
    .concurrency(4)
    .listen_and_serve("127.0.0.1", 8080);
  
  app::logging::release();
}

} // namespace prettyurl