#include "prettyurl/infra/net/http/router.hpp"
#include "prettyurl/infra/net/http/responders.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

namespace prettyurl::infra::net::http {

using namespace std::literals;

response router::not_found(request&& req) const {
  response resp;
  
  resp.status_code(core::net::http::estatus::not_found);
  resp.version(req.version());
  resp.body("Not Found"sv);
  resp.prepare_payload();
  resp.keep_alive(req.keep_alive());

  resp.header("Content-Type"sv, 
    core::net::http::helpers::make_content_type_with_charset(
      core::net::http::econtent_type::text_plain, 
      core::net::http::echarset::utf8)
  );

  return resp;
}

response router::method_not_allowed(request&& req) const {
  response resp;
  
  resp.status_code(core::net::http::estatus::method_not_allowed);
  resp.version(req.version());
  resp.body("Method Not Allowed"sv);
  resp.prepare_payload();
  resp.keep_alive(req.keep_alive());

  resp.header("Content-Type"sv, 
    core::net::http::helpers::make_content_type_with_charset(
      core::net::http::econtent_type::text_plain, 
      core::net::http::echarset::utf8)
  );

  return resp;    
}

void router::add_route(std::shared_ptr<route> route) {
  routes_.emplace_back(std::move(route));
}

response router::operator()(request&& req) {
  try {
    route_match rm;

    for (auto route : routes_) {
      if (route->match(req, rm) && rm.handler) {
        req.vars(rm.vars);
        return (*rm.handler)(std::move(req));
      } 
    }

    if (rm.error == match_error::method_mismatch) {
      return method_not_allowed(std::move(req));
    }

    if (rm.error == match_error::not_found) {
      return not_found(std::move(req));
    }
  } catch (const std::exception& e) {
    PU_LOG_ERR("an error occured while handling request ({})", e.what());
    return responders::internal_server_error(req.version(), req.keep_alive());
  }
}

} // namespace prettyurl::infra::net::http