#include "prettyurl/infra/db/repository/pg_url_repository_impl.hpp"

namespace prettyurl::infra::db::repository {

using namespace std::literals;

std::optional<std::string> pg_url_repository_impl::get_long_url(std::string_view short_url) {
  if (short_url.empty()) {
    throw std::invalid_argument("short url is empty");
  }
  
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "select long_url from url_mapping where short_url = $1 for read only";
  const auto result = session->execute_query_with_params(sql, short_url);

  if (!result.has_value()) {
    return std::nullopt;
  }
  
  return (*result)[0].get<std::string>("long_url");
}

std::optional<std::string> pg_url_repository_impl::get_short_url(std::string_view long_url) {
  if (long_url.empty()) {
    throw std::invalid_argument("long url is empty");
  }
  
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "select short_url from url_mapping where long_url = $1 limit 1 for read only";
  const auto result = session->execute_query_with_params(sql, long_url);

  if (!result.has_value()) {
    return std::nullopt;
  }

  return (*result)[0].get<std::string>("short_url");
}

void pg_url_repository_impl::add_short_url(std::string_view short_url, std::string_view long_url) {
  if (short_url.empty()) {
    throw std::invalid_argument("short url is empty");
  }

  if (long_url.empty()) {
    throw std::invalid_argument("long url is empty");
  }
  
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "insert into url_mapping(short_url, long_url) values($1, $2)";
  session->execute_query_with_params(sql, short_url, long_url);
}

std::uint64_t pg_url_repository_impl::get_last_id() {
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "select coalesce(max(id), 100000)"
                              " as id from url_mapping for read only";

  const auto result = session->execute_query(sql);

  if (!result.has_value()) {
    throw std::runtime_error("last id value is empty");
  }

  return (*result)[0].get<std::uint64_t>("id").value();
}

} // namespace prettyurl::infra::db::repository 