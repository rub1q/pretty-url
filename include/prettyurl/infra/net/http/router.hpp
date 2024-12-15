#pragma once

#include "prettyurl/infra/net/http/route.hpp"

#include <unordered_map>
#include <memory>

namespace prettyurl::infra::net::http {

class router final : public core::net::request_handler<request, response> {
public:
  router() = default;

  [[nodiscard]] response operator()(request&& req) final;

  template <typename Handler>
  void add_route(std::string_view path, const core::net::http::emethod methods, Handler&& handler) {
    if (path.empty()) {
      throw std::invalid_argument("invalid route path");
    }

    routes_.emplace(path, std::make_shared<route>(methods, std::forward<decltype(handler)>(handler)));
  }

  [[nodiscard]] std::optional<std::reference_wrapper<const route>> get(std::string_view route_path) const;

private:
  [[nodiscard]] response not_found(const unsigned version, bool keep_alive) const;
  [[nodiscard]] response method_not_allowed(const unsigned version, bool keep_alive) const;

private:
  using path = std::string;
  using routes_tab = std::unordered_map<path, std::shared_ptr<route>>;

  routes_tab routes_;
};

} // namespace prettyurl::infra::net::http