#pragma once

#include "prettyurl/core/db/base_db_session.hpp"

#include <string>
#include <optional>

namespace prettyurl::core::db {

struct kv_db_session : public base_db_session {
  [[nodiscard]] virtual std::optional<std::string> get(std::string_view key) = 0;
  [[nodiscard]] virtual bool exists(std::string_view key) = 0;

  virtual void set(std::string_view key, std::string_view value, const std::uint16_t ttl_sec = 0u) = 0;
  virtual void remove(std::string_view key) = 0;  
};

} // namespace prettyurl::core::db