#include "prettyurl/app/services/url_shortener_service.hpp"
#include "prettyurl/core/utility/url_validator.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

namespace prettyurl::app::services {

std::string url_shortener_service::shorten(std::string_view src_url) {
  if (!is_valid_url(src_url)) {
    throw std::invalid_argument("invalid url format");
  }

  const auto short_url = repo_->get_short_url(src_url);

  if (short_url.has_value()) {
    PU_LOG_INF("short url for source url already exists ({})", short_url.value());
    return short_url.value();
  }

  const auto id = id_generator_->generate();
  PU_LOG_INF("new generated id: {}", id);

  const auto encoded_str = encoder_->encode(id);
  PU_LOG_INF("encoded id: {}", encoded_str);

  repo_->add_short_url(encoded_str, src_url);

  return encoded_str;
}

bool url_shortener_service::is_valid_url(std::string_view src_url) {
  return core::utility::url_validator::is_valid(src_url);
}

} // namespace prettyurl::app::services