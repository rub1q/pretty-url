#pragma once

#include "prettyurl/core/net/request_handler.hpp"
#include "prettyurl/core/net/http/common.hpp"
#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"

namespace prettyurl::infra::net::http {

class route final {
public:
  using RouteHandlerFunc = core::net::request_handler_func<request, response>;
  
  route() = default;
  
  explicit route(const core::net::http::emethod methods, RouteHandlerFunc handler)
    : methods_(static_cast<core::net::http::emethod_ut>(methods))
    , handler_(std::move(handler)) {
  }

  route& methods(const core::net::http::emethod method) noexcept;
  route& handler(RouteHandlerFunc handler) noexcept;

  [[nodiscard]] bool is_allowed_method(const core::net::http::emethod method) const noexcept;
  [[nodiscard]] bool is_allowed_method(std::string_view method) const noexcept;
  [[nodiscard]] response handle(request&& req);
  
private:
  core::net::http::emethod_ut methods_ { 1 << static_cast<core::net::http::emethod_ut>(core::net::http::emethod::head) };  
  RouteHandlerFunc handler_;
};

} // namespace prettyurl::infra::net::http