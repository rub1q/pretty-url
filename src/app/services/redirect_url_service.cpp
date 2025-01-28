#include "prettyurl/app/services/redirect_url_service.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

namespace prettyurl::app::services {

std::optional<std::string> redirect_url_service::get_source_url(std::string_view short_url) {
  if (cache_ && cache_->exists(short_url)) {
    return cache_->get(short_url);
  }
  
  const auto long_url = repo_->get_long_url(std::move(short_url));

  if (long_url.has_value() && cache_) {
    cache_->set(short_url, long_url.value(), 3600);
  }

  return long_url;
}

} // namespace prettyurl::app::services