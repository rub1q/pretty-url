#pragma once

#include "prettyurl/core/db/query_result_set.hpp"
#include "prettyurl/core/db/base_db_session.hpp"

#include <string>
#include <optional>

namespace prettyurl::core::db {

struct sql_db_session : public base_db_session {
  virtual std::optional<query_result_set> execute_query(std::string_view query) = 0;

  virtual void begin_transaction() = 0;
  virtual void commit_transaction() = 0;
  virtual void rollback_transaction() = 0;
};

} // namespace prettyurl::core::db