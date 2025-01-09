#include "prettyurl/infra/db/pg_connect_string_builder.hpp"

#include <sstream>

namespace prettyurl::infra::db {

pg_connect_string_builder& pg_connect_string_builder::host(std::string_view host) {
  host_ = host;
  return *this;
}

pg_connect_string_builder& pg_connect_string_builder::dbname(std::string_view name) {
  db_name_ = name;
  return *this;
}

pg_connect_string_builder& pg_connect_string_builder::user(std::string_view user) {
  username_ = user;
  return *this;
}

pg_connect_string_builder& pg_connect_string_builder::password(std::string_view password) {
  password_ = password;
  return *this;
}

pg_connect_string_builder& pg_connect_string_builder::port(const std::uint16_t port) {
  port_ = port;
  return *this;
}

pg_connect_string_builder& pg_connect_string_builder::connect_timeout(const std::uint16_t timeout) {
  connect_timeout_sec_ = timeout;
  return *this;
}

std::string pg_connect_string_builder::get() const {
  if (host_.empty()) {
    throw std::runtime_error("db host does not specified");
  }

  if (db_name_.empty()) {
    throw std::runtime_error("db name does not specidied");
  }

  if (username_.empty()) {
    throw std::runtime_error("db user name does not specified");
  }

  if (password_.empty()) {
    throw std::runtime_error("db user password does not specified");
  }

  std::stringstream ss;

  // "host=localhost port=5432 dbname=name user=username password=pass connect_timeout=10"

  ss << "host=" << host_
     << " port=" << port_
     << " dbname=" << db_name_
     << " user=" << username_
     << " password=" << password_;

  if (connect_timeout_sec_.has_value()) {
    ss << " connect_timeout=" << connect_timeout_sec_.value();
  }

  return ss.str();
}

} // namespace prettyurl::infra::db