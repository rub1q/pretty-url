#include "prettyurl/infra/net/http/router.hpp"

namespace prettyurl::infra::net::http {

using namespace std::literals;

response router::not_found(const unsigned version, bool keep_alive) const {
  static constexpr auto response_func = [] (const unsigned version, bool keep_alive) {
    response resp;
    
    resp.status_code(core::net::http::estatus::not_found);
    resp.version(version);
    resp.body("Not Found"sv);
    resp.prepare_payload();
    resp.keep_alive(keep_alive);

    // TODO: change string literal header to enum constant

    resp.header("Content-Type"sv, 
      core::net::http::helpers::make_content_type_with_charset(
        core::net::http::econtent_type::application_json, 
        core::net::http::echarset::utf8)
    );

    return resp;
  };

  return response_func(version, keep_alive);
}

response router::method_not_allowed(const unsigned version, bool keep_alive) const {
  static constexpr auto response_func = [] (const unsigned version, bool keep_alive) {
    response resp;
    
    resp.status_code(core::net::http::estatus::method_not_allowed);
    resp.version(version);
    resp.body("Method Not Allowed"sv);
    resp.prepare_payload();
    resp.keep_alive(keep_alive);

    // TODO: change string literal header to enum constant

    resp.header("Content-Type"sv, 
      core::net::http::helpers::make_content_type_with_charset(
        core::net::http::econtent_type::application_json, 
        core::net::http::echarset::utf8)
    );

    return resp;    
  };

  return response_func(version, keep_alive);
}

void router::add_route(std::string_view path, const core::net::http::emethod methods, route::RouteHandlerFunc::HandlerFunc handler) {
  routes_.emplace(path, std::make_shared<route>(methods, std::move(handler)));
}

response router::operator()(request&& req) {
  const auto route_it = routes_.find(req.target().data());
  
  if (route_it == routes_.cend()) { // TODO
    return not_found(req.version(), req.keep_alive());
  }

  if (!route_it->second->is_allowed_method(req.method())) {
    return method_not_allowed(req.version(), req.keep_alive());
  }

  return route_it->second->handle(std::move(req));
}

} // namespace prettyurl::infra::net::http