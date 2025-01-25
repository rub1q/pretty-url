#pragma once

#include "prettyurl/infra/net/http/route.hpp"

#include <unordered_map>
#include <memory>
#include <optional>

namespace prettyurl::infra::net::http {

class router final : public core::net::request_handler<request, response> {
public:
  router() = default;

  [[nodiscard]] response operator()(request&& req) final;

  template <typename Handler>
  void add_route(std::string_view path, const core::net::http::emethod methods, Handler&& handler) {
    routes_.emplace_back(std::make_shared<route>(path, methods, std::forward<decltype(handler)>(handler)));
  }

  void add_route(std::shared_ptr<route> route);

private:
  [[nodiscard]] response not_found(request&& req) const;
  [[nodiscard]] response method_not_allowed(request&& req) const;

private:
  std::vector<std::shared_ptr<route>> routes_;
};

} // namespace prettyurl::infra::net::http