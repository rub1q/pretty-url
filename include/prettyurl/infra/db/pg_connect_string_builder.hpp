#pragma once

#include <string>
#include <cstdint>
#include <optional>

namespace prettyurl::infra::db {

class pg_connect_string_builder {
public:
  pg_connect_string_builder() = default;

  pg_connect_string_builder& host(std::string_view host);
  pg_connect_string_builder& dbname(std::string_view name);
  pg_connect_string_builder& user(std::string_view user);
  pg_connect_string_builder& password(std::string_view password);

  pg_connect_string_builder& port(const std::uint16_t port);
  pg_connect_string_builder& connect_timeout(const std::uint16_t timeout);

  [[nodiscard]] std::string get() const;

private:
  std::uint16_t port_;
  std::optional<std::uint16_t> connect_timeout_sec_;

  std::string host_;
  std::string db_name_;
  std::string username_;
  std::string password_; 
};

} // namespace prettyurl::infra::db 