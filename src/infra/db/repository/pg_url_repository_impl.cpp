#include "prettyurl/infra/db/repository/pg_url_repository_impl.hpp"

namespace prettyurl::infra::db::repository {

using namespace std::literals;

std::optional<std::string> pg_url_repository_impl::get_long_url(std::string_view short_url) {
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "select long_url from url_mapping where short_url = $1";
  const auto result = session->execute_query(sql, short_url);
  
  return (*result)[0].get<std::string>("long_url");
}

std::optional<std::string> pg_url_repository_impl::get_short_url(std::string_view long_url) {
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "select short_url from url_mapping where long_url = $1 limit 1";
  const auto result = session->execute_query(sql, long_url);

  return (*result)[0].get<std::string>("short_url");
}

void pg_url_repository_impl::add_short_url(std::string_view short_url, std::string_view long_url) {
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "insert into url_mapping(short_url, long_url) values($1, $2)";
  
  try {
    session->begin_transaction();
    session->execute_query(sql, short_url, long_url);
    session->commit_transaction();
  } catch (const std::exception& e) {
    session->rollback_transaction();
  }
}

std::uint64_t pg_url_repository_impl::get_last_id() {
  auto session = dbsm_.get_session(10s);

  static constinit auto sql = "select coalesce(max(id), currval('custom_url_id_seq'::regclass))"
                              " as id from url_mapping";

  const auto result = session->execute_query(sql);

  return (*result)[0].get<std::uint64_t>("id").value();
}

} // namespace prettyurl::infra::db::repository 