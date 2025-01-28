#pragma once

#include "prettyurl/core/db/sql_db_session.hpp"
#include "prettyurl/core/utility/string_utils.hpp"
#include "prettyurl/core/config/db_config.hpp"

#include <memory>
#include <unordered_set>

#include <pqxx/pqxx>

namespace prettyurl::infra::db {

class pg_session final : public core::db::sql_db_session {
public:
  ~pg_session();

  pg_session(const pg_session&) = default;
  pg_session(pg_session&&) = default;

  pg_session& operator=(const pg_session&) = default;
  pg_session& operator=(pg_session&&) = default;

  explicit pg_session(const core::config::db_config& cfg);
  
  [[nodiscard]] bool is_connected() const final;

  void reconnect() final;

  void begin_transaction() final;
  void commit_transaction() final;
  void rollback_transaction() final;

  template <typename FirstArg, typename... RestArgs>
  std::optional<core::db::query_result_set> execute_query_with_params(std::string_view query, FirstArg&& first_arg, RestArgs&&... args) {
    return execute_query_impl(query, std::forward<FirstArg>(first_arg), std::forward<RestArgs>(args)...);
  }

  std::optional<core::db::query_result_set> execute_query(std::string_view query) final;

private:
  void connect() final;
  void disconnect() final;

  [[nodiscard]] std::pair<std::string, bool> prepare_statement(std::string_view query);
  [[nodiscard]] core::db::query_result_set to_query_result_set(const pqxx::result& result) const;

  template <typename... Args>
  std::optional<core::db::query_result_set> execute_query_impl(std::string_view query, Args&&... args) {
    try {
      if (!trn_) {
        begin_transaction();
      }

      pqxx::result res;

      if constexpr (sizeof...(args) > 0) {
        const auto [statement, is_prepared] = prepare_statement(query);
        
        if (!is_prepared) {
          conn_->prepare(statement, query.data());
        }

        res = trn_->exec_prepared(statement, std::forward<Args>(args)...);
      } else {
        res = trn_->exec(query.data());
      }

      commit_transaction();

      if (res.empty()) {
        return std::nullopt;
      }

      return to_query_result_set(res);
    } catch (const std::exception& e) {
      rollback_transaction();

      throw std::runtime_error("an error occured while executing sql query (" + std::string(e.what()) + ")");
    }
  }  

private:
  std::unique_ptr<pqxx::connection> conn_;
  std::unique_ptr<pqxx::transaction<>> trn_;

  std::unordered_set<std::string> prepared_statements_;

  std::string conn_string_;
};

} // namespace prettyurl::infra::db