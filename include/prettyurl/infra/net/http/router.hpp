#pragma once

#include "prettyurl/infra/net/http/route.hpp"

#include <unordered_map>
#include <memory>

namespace prettyurl::infra::net::http {

class router final : public core::net::request_handler<request, response> {
public:
  router() = default;

  [[nodiscard]] response operator()(request&& req) final;

  void add_route(std::string_view path, const core::net::http::emethod methods, route::RouteHandlerFunc::HandlerFunc handler);

private:
  [[nodiscard]] response not_found(const unsigned version, bool keep_alive) const;
  [[nodiscard]] response method_not_allowed(const unsigned version, bool keep_alive) const;

private:
  using path = std::string;
  using routes_tab = std::unordered_map<path, std::shared_ptr<route>>;

  routes_tab routes_;
};

} // namespace prettyurl::infra::net::http