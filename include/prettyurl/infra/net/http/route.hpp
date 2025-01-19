#pragma once

#include "prettyurl/core/net/request_handler.hpp"
#include "prettyurl/core/net/http/common.hpp"
#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"
#include "prettyurl/core/net/http/route_matcher.hpp"

#include <memory>
#include <unordered_map>

namespace prettyurl::infra::net::http {

struct route_match;

class route final {
public:
  using handler_func = core::net::request_handler_func<request, response>;
  
  route() = default;
  
  template <typename Handler>
  explicit route(std::string_view path, const core::net::http::emethod methods, Handler&& handler)
    : handler_(std::make_unique<handler_func>(std::forward<decltype(handler)>(handler)))
    , matcher_(path) {
    methods_ |= static_cast<std::uint32_t>(methods);
  }

  route& methods(const core::net::http::emethod method) noexcept;
  
  template <typename Handler>
  route& handler(Handler&& handler) noexcept {
    if (handler_) {
      handler_.release();
    }
    
    handler_ = std::make_unique<handler_func>(std::forward<decltype(handler)>(handler));
    return *this;    
  }

  [[nodiscard]] bool match(const request& req, route_match& rm) const;

  [[nodiscard]] bool is_allowed_method(const core::net::http::emethod method) const noexcept;
  [[nodiscard]] bool is_allowed_method(std::string_view method) const noexcept;
  [[nodiscard]] response handle(request&& req);
  
private:
  std::uint32_t methods_ { static_cast<std::uint32_t>(core::net::http::emethod::head) };
  std::unique_ptr<handler_func> handler_ { nullptr };

  core::net::http::route_matcher matcher_;
};

enum class match_error : std::uint8_t {
  ok,
  method_mismatch,
  not_found
};

struct route_match final {
  match_error error { match_error::ok };

  route::handler_func* handler { nullptr };
  std::unordered_map<std::string, std::string> vars;
};

} // namespace prettyurl::infra::net::http