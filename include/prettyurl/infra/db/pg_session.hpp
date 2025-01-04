#pragma once

#include "prettyurl/core/db/base_db_session.hpp"
#include "prettyurl/core/utility/string_utils.hpp"

#include <memory>
#include <pqxx/pqxx>

namespace prettyurl::infra::db {

class pg_session final : public core::db::base_db_session {
public:
  ~pg_session();

  pg_session(const pg_session&) = default;
  pg_session(pg_session&&) = default;

  pg_session& operator=(const pg_session&) = default;
  pg_session& operator=(pg_session&&) = default;

  explicit pg_session(std::string_view conn_string)
    : conn_string_(std::move(conn_string)) {
    connect();
  }

  [[nodiscard]] bool is_connected() const final;

  void reconnect() final;

  void begin_transaction() final;
  void commit_transaction() final;
  void rollback_transaction() final;

  template <typename... Args>
  std::optional<core::db::query_result_set> execute_query(std::string_view query, Args&&... args) {
    query_params_.clear();

    constexpr auto size = sizeof... (args);
     
    if constexpr (size > 0) {
      query_params_.reserve(size);
      (query_params_.push_back(core::utility::to_string(std::forward<decltype(args)>(args))), ...);
    }

    return execute(query);
  }

private:
  void connect() final;
  void disconnect() final;

  std::optional<core::db::query_result_set> execute(std::string_view query) final;

private:
  std::unique_ptr<pqxx::connection> conn_;
  std::unique_ptr<pqxx::transaction<>> trn_;

  std::vector<std::string> query_params_;

  std::string conn_string_;
};

} // namespace prettyurl::infra::db