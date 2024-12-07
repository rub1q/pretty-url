#include "prettyurl/app/handlers/redirect_handler.hpp"

namespace prettyurl::app::handlers {

infra::net::http::response redirect_handler::operator()(infra::net::http::request&& req) {
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
  resp.header("Location"sv, "https://www.google.com/"sv); // TODO

  return resp;    
}

} // namespace prettyurl::app::handlers