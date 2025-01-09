#pragma once

#include <string>
#include <cstdint>

namespace prettyurl::core::config {

struct db_config final {
  std::uint16_t port;
  std::uint16_t connect_timeout_sec { 10u };
  std::uint32_t sessions_pool_size { 5u };
  std::string ip;
  std::string db_name;
  std::string username;
  std::string password;
};

} // namespace prettyurl::core::config