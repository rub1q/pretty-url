#pragma once

#include "prettyurl/core/net/request_handler.hpp"
#include "prettyurl/core/net/http/common.hpp"
#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"

namespace prettyurl::infra::net::http {

class route final {
public:
  using handler_func = core::net::request_handler_func<request, response>;
  
  route() = default;
  
  explicit route(const core::net::http::emethod methods, handler_func handler)
    : methods_(static_cast<std::uint32_t>(methods))
    , handler_(std::move(handler)) {
  }

  route& methods(const core::net::http::emethod method) noexcept;
  route& handler(handler_func handler) noexcept;

  [[nodiscard]] bool is_allowed_method(const core::net::http::emethod method) const noexcept;
  [[nodiscard]] bool is_allowed_method(std::string_view method) const noexcept;
  [[nodiscard]] response handle(request&& req);
  
private:
  std::uint32_t methods_;  
  handler_func handler_;
};

} // namespace prettyurl::infra::net::http