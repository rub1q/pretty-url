#include "prettyurl/infra/net/http/router.hpp"
#include "prettyurl/infra/net/http/responders.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

namespace prettyurl::infra::net::http {

using namespace std::literals;

response router::not_found(const unsigned version, bool keep_alive) const {
  response resp;
  
  resp.status_code(core::net::http::estatus::not_found);
  resp.version(version);
  resp.body("Not Found"sv);
  resp.prepare_payload();
  resp.keep_alive(keep_alive);

  resp.header("Content-Type"sv, 
    core::net::http::helpers::make_content_type_with_charset(
      core::net::http::econtent_type::text_plain, 
      core::net::http::echarset::utf8)
  );

  return resp;
}

response router::method_not_allowed(const unsigned version, bool keep_alive) const {
  response resp;
  
  resp.status_code(core::net::http::estatus::method_not_allowed);
  resp.version(version);
  resp.body("Method Not Allowed"sv);
  resp.prepare_payload();
  resp.keep_alive(keep_alive);

  resp.header("Content-Type"sv, 
    core::net::http::helpers::make_content_type_with_charset(
      core::net::http::econtent_type::text_plain, 
      core::net::http::echarset::utf8)
  );

  return resp;    
}

std::optional<std::reference_wrapper<const route>> router::get(std::string_view route_path) const {
  if (!routes_.contains(route_path.data())) {
    return std::nullopt;
  }

  return std::cref(*(routes_.at(route_path.data()).get()));
}

response router::operator()(request&& req) {
  try {
    const auto route_it = routes_.find(std::string(req.target()));

    if (route_it == routes_.cend()) { // TODO
      return not_found(req.version(), req.keep_alive());
    }

    if (!route_it->second->is_allowed_method(req.method())) {
      return method_not_allowed(req.version(), req.keep_alive());
    }

    return route_it->second->handle(std::move(req));
  } catch (const std::exception& e) {
    PU_LOG_ERR("an error occured while handling request ({})", e.what());
    return responders::internal_server_error(req.version(), req.keep_alive());
  }
}

} // namespace prettyurl::infra::net::http