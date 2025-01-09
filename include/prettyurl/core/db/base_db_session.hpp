#pragma once

#include "prettyurl/core/db/query_result_set.hpp"

#include <string>
#include <optional>

namespace prettyurl::core::db {

class base_db_session {
public:
  virtual ~base_db_session() = default;

  [[nodiscard]] virtual bool is_connected() const = 0;
  
  virtual std::optional<query_result_set> execute_query(std::string_view query) = 0;

  virtual void reconnect() = 0;

  virtual void begin_transaction() = 0;
  virtual void commit_transaction() = 0;
  virtual void rollback_transaction() = 0;

protected:
  virtual void connect() = 0;
  virtual void disconnect() = 0;
};

} // namespace prettyurl::core::db