#pragma once

#include <cstdint>
#include <string>
#include <algorithm>

namespace prettyurl::core::net::http {

enum class emethod : std::uint32_t {
  unknown = (1 << 0),
  delete_ = (1 << 1),
  get     = (1 << 2),
  head    = (1 << 3), 
  post    = (1 << 4),
  put     = (1 << 5),
  patch   = (1 << 6)
};

enum class econtent_type : std::uint8_t {
  text_plain,
  text_html,
  text_css,
  text_javascript,
  
  application_json,
  application_xml,
  application_soap_xml,
  application_octet_stream
};

enum class echarset : std::uint8_t {
  utf8
};

enum class estatus : std::uint16_t {
  unknown = 0,

  continue_                           = 100,
  switching_protocols                 = 101,

  processing                          = 102,

  ok                                  = 200,
  created                             = 201,
  accepted                            = 202,
  non_authoritative_information       = 203,
  no_content                          = 204,
  reset_content                       = 205,
  partial_content                     = 206,
  multi_status                        = 207,
  already_reported                    = 208,
  im_used                             = 226,

  multiple_choices                    = 300,
  moved_permanently                   = 301,
  found                               = 302,
  see_other                           = 303,
  not_modified                        = 304,
  use_proxy                           = 305,
  temporary_redirect                  = 307,
  permanent_redirect                  = 308,

  bad_request                         = 400,
  unauthorized                        = 401,
  payment_required                    = 402,
  forbidden                           = 403,
  not_found                           = 404,
  method_not_allowed                  = 405,
  not_acceptable                      = 406,
  proxy_authentication_required       = 407,
  request_timeout                     = 408,
  conflict                            = 409,
  gone                                = 410,
  length_required                     = 411,
  precondition_failed                 = 412,
  payload_too_large                   = 413,
  uri_too_long                        = 414,
  unsupported_media_type              = 415,
  range_not_satisfiable               = 416,
  expectation_failed                  = 417,
  misdirected_request                 = 421,
  unprocessable_entity                = 422,
  locked                              = 423,
  failed_dependency                   = 424,
  upgrade_required                    = 426,
  precondition_required               = 428,
  too_many_requests                   = 429,
  request_header_fields_too_large     = 431,
  connection_closed_without_response  = 444,
  unavailable_for_legal_reasons       = 451,
  client_closed_request               = 499,

  internal_server_error               = 500,
  not_implemented                     = 501,
  bad_gateway                         = 502,
  service_unavailable                 = 503,
  gateway_timeout                     = 504,
  http_version_not_supported          = 505,
  variant_also_negotiates             = 506,
  insufficient_storage                = 507,
  loop_detected                       = 508,
  not_extended                        = 510,
  network_authentication_required     = 511,
  network_connect_timeout_error       = 599
};

struct method final {
  method() = delete;

  [[nodiscard]] static constexpr emethod from_string(std::string_view method_str) {
    if (method_str.empty()) {
      return emethod::unknown;
    }

    using namespace std::literals;

    std::string method(method_str);

    std::transform(method.begin(), method.end(), method.begin(), 
                  [](unsigned char c) { return std::tolower(c); });

    if (method == "head"sv) {
      return emethod::head;
    } else if (method == "get"sv) {
      return emethod::get;
    } else if (method == "post"sv) {
      return emethod::post;
    } else if (method == "put"sv) {
      return emethod::put;
    } else if (method == "delete"sv) {
      return emethod::delete_;
    } else if (method == "patch"sv) {
      return emethod::patch;
    }

    return emethod::unknown;
  }

  [[nodiscard]] static constexpr std::string to_string(const emethod method) {
    switch (method) {
      case emethod::head :
        return "HEAD";
      case emethod::get :
        return "GET";
      case emethod::post :
        return "POST";
      case emethod::put :
        return "PUT";
      case emethod::delete_ :
        return "DELETE";
      case emethod::patch :
        return "PATCH";

      default : 
        return "UNKNOWN";
    }
  }
};

struct content_type final {
  content_type() = delete;

  [[nodiscard]] static constexpr std::string to_string(const econtent_type ct) noexcept {
    switch (ct) {
      case econtent_type::text_plain :
        return "text/plain";
      case econtent_type::text_html :
        return "text/html";
      case econtent_type::text_css :
        return "text/css";
      case econtent_type::text_javascript :
        return "text/javascript";
      case econtent_type::application_json :
        return "application/json";
      case econtent_type::application_xml :
        return "application/xml";
      case econtent_type::application_soap_xml :
        return "application/soap+xml";
      case econtent_type::application_octet_stream :
        return "application/octet-stream";

      default : return "";
    }
  }
};

struct charset final {
  charset() = delete;

  [[nodiscard]] static constexpr std::string to_string(const echarset charset) noexcept {
    switch (charset) {
      case echarset::utf8 :
        return "UTF-8";

      default : return "";
    }
  }
};

namespace helpers {

[[nodiscard]] constexpr inline std::string make_content_type_with_charset(const econtent_type ct, const echarset charset) noexcept {
  return content_type::to_string(ct).append("; charset=").append(charset::to_string(charset));
}

} // namespace helpers
} // namespace prettyurl::core::net::http

inline constexpr prettyurl::core::net::http::emethod
operator|(prettyurl::core::net::http::emethod m1, prettyurl::core::net::http::emethod m2) {
  const auto val_1 = (static_cast<std::uint32_t>(m1));
  const auto val_2 = (static_cast<std::uint32_t>(m2));
  
  return static_cast<prettyurl::core::net::http::emethod>(val_1 | val_2);
}