#include "prettyurl/app/handlers/url_shortener_handler.hpp"

#include <rapidjson/document.h>

namespace prettyurl::app::handlers {

infra::net::http::response url_shortener_handler::operator()(infra::net::http::request&& req) {
  rapidjson::Document root;
  root.Parse(req.body().data());

  if (root.HasParseError()) {
    // return bad request
  }

  if (!root.HasMember("longUrl")) {
    // return bad request
    
  }

  if (!root["longUrl"].IsString()) {
    // return bad request
  }

  auto long_url = root["longUrl"].GetString();

  // TODO: validate long url
  
  infra::net::http::response resp {};

  return resp;
}

} // namespace prettyurl::app::handlers