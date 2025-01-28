#pragma once

#include <optional>
#include <string>

namespace prettyurl::core::caching {

struct base_cache {
  virtual ~base_cache() = default;

  [[nodiscard]] virtual std::optional<std::string> get(std::string_view key) = 0;
  [[nodiscard]] virtual bool exists(std::string_view key) = 0;

  virtual void set(std::string_view key, std::string_view value, const std::uint16_t ttl_sec = 0u) = 0;
  virtual void remove(std::string_view key) = 0;
};

} // namespace prettyurl::core::caching