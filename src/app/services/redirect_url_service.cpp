#include "prettyurl/app/services/redirect_url_service.hpp"

namespace prettyurl::app::services {

std::optional<std::string> redirect_url_service::get_source_url(std::string_view short_url) {
  return repo_->get_long_url(std::move(short_url));
}

} // namespace prettyurl::app::services