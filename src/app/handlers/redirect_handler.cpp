#include "prettyurl/app/handlers/redirect_handler.hpp"
#include "prettyurl/infra/net/http/responders.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

namespace prettyurl::app::handlers {

infra::net::http::response redirect_handler::operator()(infra::net::http::request&& req) {
  try {
    if (!req.vars().contains("short_url")) {
      return infra::net::http::responders::bad_request(req.version(), req.keep_alive(), R"("message": "Invalid Route Path")");
    }

    const auto src_url = service_->get_source_url(req.vars().at("short_url"));

    if (!src_url.has_value()) {
      return infra::net::http::responders::not_found(req.version(), req.keep_alive(), R"("message": "Source Url Not Found")");
    }
    
    infra::net::http::response resp { core::net::http::estatus::found };

    using namespace std::literals;

    resp.version(req.version());
    resp.keep_alive(req.keep_alive());

    resp.header("Content-Type"sv, 
      core::net::http::helpers::make_content_type_with_charset(
        core::net::http::econtent_type::application_json, 
        core::net::http::echarset::utf8)
    );

    resp.header("Cache-Control"sv, "no-cache"sv);
    resp.header("Location"sv, src_url.value());

    return resp;
  } catch (const std::exception& e) {
    PU_LOG_ERR("an error occured while processing url redirection ({})", e.what());
    return infra::net::http::responders::internal_server_error(req.version(), req.keep_alive());
  } catch (...) {
    PU_LOG_ERR("an unknown error occured while processing url redirection");
    return infra::net::http::responders::internal_server_error(req.version(), req.keep_alive());
  }
}

} // namespace prettyurl::app::handlers