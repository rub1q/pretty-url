#pragma once

#include "prettyurl/core/net/request_handler.hpp"
#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"

namespace prettyurl::app::handlers {

class url_shortener_handler : public core::net::request_handler<infra::net::http::request, infra::net::http::response> {
public:
  infra::net::http::response operator()(infra::net::http::request&& req);
};

} // namespace prettyurl::app::handlers