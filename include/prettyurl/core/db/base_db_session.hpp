#pragma once

#include <string>

namespace prettyurl::core::db {

class base_db_session {
public:
  virtual ~base_db_session() = default;

  [[nodiscard]] virtual bool is_connected() const = 0;
  
  virtual void reconnect() = 0;

protected:
  virtual void connect() = 0;
  virtual void disconnect() = 0;
};

} // namespace prettyurl::core::db