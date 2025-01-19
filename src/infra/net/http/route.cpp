#include "prettyurl/infra/net/http/route.hpp"

namespace prettyurl::infra::net::http {

route& route::methods(const core::net::http::emethod method) noexcept {
  methods_ |= static_cast<std::uint32_t>(method);
  return *this;
}

bool route::is_allowed_method(const core::net::http::emethod method) const noexcept {
  return methods_ & (static_cast<std::uint32_t>(method));
}

bool route::is_allowed_method(std::string_view method) const noexcept {
  const auto method_int = core::net::http::method::from_string(method);
  return is_allowed_method(method_int);  
}

bool route::match(const request& req, route_match& rm) const {
  if (matcher_.match(req.target(), rm.vars)) {
    if (is_allowed_method(req.method())) {
      rm.error = match_error::ok;
      rm.handler = handler_.get();

      return true;
    }

    rm.error = match_error::method_mismatch;
  } else {
    rm.error = match_error::not_found;
  }

  return false;
}

response route::handle(request&& req) {
  if (!handler_) {
    throw std::runtime_error("route handler is null");
  }
  
  return (*handler_)(std::move(req));
}

} // namespace prettyurl::infra::net::http