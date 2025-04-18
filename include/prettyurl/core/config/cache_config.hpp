#pragma once

#include <string>
#include <cstdint>

namespace prettyurl::core::config {

struct cache_config final {
  std::uint16_t port;
  std::uint16_t connect_timeout_sec { 10u };
  std::string ip;
  std::string password;
};

} // namespace prettyurl::core::config