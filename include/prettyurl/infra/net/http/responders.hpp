#pragma once

#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"

namespace prettyurl::infra::net::http {
namespace responders {

using namespace std::literals;

[[nodiscard]] inline response not_found(const unsigned version, bool keep_alive, std::string_view text = R"({"message": "Not Found"})"sv) {
  response resp;
  
  resp.status_code(core::net::http::estatus::not_found);
  resp.version(version);
  resp.body(text);
  resp.prepare_payload();
  resp.keep_alive(keep_alive);

  resp.header("Content-Type"sv, 
    core::net::http::helpers::make_content_type_with_charset(
      core::net::http::econtent_type::application_json, 
      core::net::http::echarset::utf8)
  );

  return resp; 
}

[[nodiscard]] inline response bad_request(const unsigned version, bool keep_alive, std::string_view text = R"({"message": "Bad Request"})"sv) {
  response resp;
  
  resp.status_code(core::net::http::estatus::bad_request);
  resp.version(version);
  resp.body(text);
  resp.prepare_payload();
  resp.keep_alive(keep_alive);

  resp.header("Content-Type"sv, 
    core::net::http::helpers::make_content_type_with_charset(
      core::net::http::econtent_type::application_json, 
      core::net::http::echarset::utf8)
  );

  return resp;
}

[[nodiscard]] inline response internal_server_error(const unsigned version, bool keep_alive, std::string_view text = R"({"message": "Internal Server Error"})"sv) {
  response resp;
  
  resp.status_code(core::net::http::estatus::internal_server_error);
  resp.version(version);
  resp.body(text);
  resp.prepare_payload();
  resp.keep_alive(keep_alive);

  resp.header("Content-Type"sv, 
    core::net::http::helpers::make_content_type_with_charset(
      core::net::http::econtent_type::application_json, 
      core::net::http::echarset::utf8)
  );

  return resp;
}

} // namespace responders
} // namespace prettyurl::infra::net::responders