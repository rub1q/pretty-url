#include "prettyurl/app.hpp"

#include "prettyurl/core/db/db_session_manager.hpp"
#include "prettyurl/core/encoding/base62_encoder.hpp"

#include "prettyurl/app/logging/log-inl.hpp"
#include "prettyurl/app/handlers/redirect_handler.hpp"
#include "prettyurl/app/handlers/url_shortener_handler.hpp"
#include "prettyurl/app/services/url_shortener_service.hpp"
#include "prettyurl/app/services/redirect_url_service.hpp"

#include "prettyurl/infra/db/repository/pg_url_repository_impl.hpp"
#include "prettyurl/infra/db/pg_session.hpp"
#include "prettyurl/infra/logging/loggers/console_logger.hpp"
#include "prettyurl/infra/logging/loggers/file_logger.hpp"
#include "prettyurl/infra/net/http/server.hpp"
#include "prettyurl/infra/net/http/router.hpp"
#include "prettyurl/infra/db/auto_inc_id_generator.hpp"
#include "prettyurl/infra/caching/redis_cache.hpp"

namespace prettyurl {

using namespace std::literals;

void application::run(const core::config::app_config& cfg) {
  auto clogger = app::logging::create<infra::logging::loggers::console_logger>("console");
  auto flogger = app::logging::create<infra::logging::loggers::file_logger>("file", cfg.logging.path);

  app::logging::set_default_logger(flogger);
  app::logging::set_log_level(core::logging::level::from_string(cfg.logging.level));

  PU_LOG_INF("start init app");
  PU_LOG_INF_TO("console"_logger, "start init app");

  PU_LOG_INF("connecting to db...");
  PU_LOG_INF_TO("console"_logger, "connecting to db...");

  core::db::db_session_manager<infra::db::pg_session> dbs_man(cfg.db.sessions_pool_size, cfg.db);

  auto url_repo = std::make_shared<infra::db::repository::pg_url_repository_impl>(dbs_man);

  auto encoder = std::make_shared<core::encoding::base62_encoder>();
  auto id_generator = std::make_shared<infra::db::auto_inc_id_generator>(url_repo);

  auto cache = std::make_shared<infra::caching::redis_cache>(cfg.cache);

  auto shorten_service = std::make_shared<app::services::url_shortener_service>(url_repo, cache, std::move(encoder), std::move(id_generator));
  auto redirect_service = std::make_shared<app::services::redirect_url_service>(url_repo, cache);

  infra::net::http::router router;

  router.add_route("/api/v1/{short_url}"sv, 
    core::net::http::emethod::get, 
    app::handlers::redirect_handler{ std::move(redirect_service) }
  );

  router.add_route("/api/v1/shorten"sv, 
    core::net::http::emethod::post,
    app::handlers::url_shortener_handler{ std::move(shorten_service) }
  );

  infra::net::http::server(std::move(router))
    .concurrency(cfg.server.thread_pool_size)
    .read_timeout(cfg.server.read_timeout_sec)
    .write_timeout(cfg.server.write_timeout_sec)
    .listen_and_serve(cfg.server.ip, cfg.server.port);
  
  app::logging::release();
}

} // namespace prettyurl