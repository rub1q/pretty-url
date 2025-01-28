#pragma once

#include "prettyurl/core/caching/base_cache.hpp"
#include "prettyurl/core/config/cache_config.hpp"

#include <memory>
#include <hiredis/hiredis.h>

namespace prettyurl::infra::caching {

class redis_cache final : public core::caching::base_cache {
public:
  explicit redis_cache(const core::config::cache_config& cfg)
    : cfg_(cfg) {
    connect();
  }

  [[nodiscard]] std::optional<std::string> get(std::string_view key) final;
  [[nodiscard]] bool exists(std::string_view key) final;

  void set(std::string_view key, std::string_view value, const std::uint16_t ttl_sec = 0u) final;
  void remove(std::string_view key) final;

private:
  void connect();
  void reconnect();

private:
  std::shared_ptr<redisContext> redis_ctx_;
  const core::config::cache_config& cfg_;
};

} // namespace prettyurl::infra::caching