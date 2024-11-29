#pragma once 

#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>

#include "prettyurl/infra/net/common.hpp"

namespace prettyurl::infra::net::http {

namespace http = boost::beast::http;

using flat_buffer = boost::beast::flat_buffer;

using http_string_request = http::request<http::string_body>;
using http_string_response = http::response<http::string_body>;

} // namespace prettyurl::infra::net::http