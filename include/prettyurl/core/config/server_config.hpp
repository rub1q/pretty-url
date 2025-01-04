#pragma once

#include <string>
#include <cstdint>
#include <chrono>

namespace prettyurl::core::config {

struct server_config final {
  std::uint16_t thread_pool_size { 4u };
  std::uint16_t port { 8080u };

  std::chrono::seconds read_timeout_sec { std::chrono::seconds(10) };
  std::chrono::seconds write_timeout_sec { std::chrono::seconds(10) };

  std::string ip { "0.0.0.0" };
};

} // namespace prettyurl::core::config