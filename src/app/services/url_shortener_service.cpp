#include "prettyurl/app/services/url_shortener_service.hpp"

namespace prettyurl::app::services {

std::string url_shortener_service::shorten(std::string_view src_url) {
  if (!is_valid_url(src_url)) {
    //
  }

  return "hgbQwX8";
}

bool url_shortener_service::is_valid_url(std::string_view src_url) {
  return true;
}

} // namespace prettyurl::app::services