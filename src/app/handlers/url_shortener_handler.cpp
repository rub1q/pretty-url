#include "prettyurl/app/handlers/url_shortener_handler.hpp"
#include "prettyurl/app/logging/log-inl.hpp"
#include "prettyurl/infra/net/http/responders.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace prettyurl::app::handlers {

using namespace std::literals;

infra::net::http::response url_shortener_handler::operator()(infra::net::http::request&& req) {
  try {
    rapidjson::Document root;
    root.Parse(req.body().data());

    if (root.HasParseError() || !root.HasMember("longUrl") || !root["longUrl"].IsString()) {
      return infra::net::http::responders::bad_request(req.version(), req.keep_alive(), R"("message": "Invalid Request Body")");
    }

    const auto long_url = root["longUrl"].GetString();
    const auto short_url = service_->shorten(long_url);

    rapidjson::Document resp_body;
    resp_body.SetObject();

    rapidjson::Value short_url_val(short_url.c_str(), resp_body.GetAllocator());
    resp_body.AddMember("shortUrl", short_url_val, resp_body.GetAllocator());

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    
    resp_body.Accept(writer);    

    infra::net::http::response resp { core::net::http::estatus::ok };

    resp.keep_alive(req.keep_alive());
    resp.version(req.version());
    resp.body(sb.GetString());
    resp.header("Content-Type"sv, 
      core::net::http::helpers::make_content_type_with_charset(
        core::net::http::econtent_type::application_json,
        core::net::http::echarset::utf8
      )
    );

    resp.header("Cache-Control"sv, "no-cache"sv);
    resp.prepare_payload();

    return resp;

  } catch (const std::exception& e) {
    PU_LOG_ERR("an error occured while processing url shorten ({})", e.what());
    return infra::net::http::responders::internal_server_error(req.version(), req.keep_alive());
  } catch (...) {
    PU_LOG_ERR("an unknown error occured while processing url shorten");
    return infra::net::http::responders::internal_server_error(req.version(), req.keep_alive());
  }
}

} // namespace prettyurl::app::handlers