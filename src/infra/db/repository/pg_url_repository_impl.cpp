#include "prettyurl/infra/db/repository/pg_url_repository_impl.hpp"

namespace prettyurl::infra::db::repository {

std::optional<std::string> pg_url_repository_impl::get_long_url(std::string_view short_url) {
  return {};
}

std::optional<std::string> pg_url_repository_impl::get_short_url(std::string_view long_url) {
  return {};
}

void pg_url_repository_impl::add_short_url(std::string_view short_url, std::string_view long_url) {
  return;
}

std::uint64_t pg_url_repository_impl::get_last_id() {
  return 0;
}

} // namespace prettyurl::infra::db::repository 