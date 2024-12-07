#pragma once

#include "prettyurl/core/net/request_handler.hpp"
#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"
#include "prettyurl/app/services/url_shortener_service.hpp"

namespace prettyurl::app::handlers {

class url_shortener_handler final : public core::net::request_handler<infra::net::http::request, infra::net::http::response> {
public:
  explicit url_shortener_handler(std::shared_ptr<services::url_shortener_service> service)
    : service_(std::move(service)) {
  }

  infra::net::http::response operator()(infra::net::http::request&& req) final;

private:
  std::shared_ptr<services::url_shortener_service> service_;
};

} // namespace prettyurl::app::handlers