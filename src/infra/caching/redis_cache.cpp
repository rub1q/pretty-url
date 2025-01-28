#include "prettyurl/infra/caching/redis_cache.hpp"
#include "prettyurl/core/utility/string_utils.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

namespace prettyurl::infra::caching {
 
void redis_cache::connect() {
  const auto ctx = redisConnectWithTimeout(cfg_.ip.c_str(), cfg_.port, timeval{ .tv_sec = cfg_.connect_timeout_sec });
  if (!ctx || ctx->err) {
    if (ctx) {
      redisFree(ctx);
    }

    throw std::runtime_error(core::utility::concat("failed to connect to redis (", std::string(ctx->errstr), ")"));
  }

  auto cmd = redisCommand(ctx, "AUTH %s", cfg_.password.c_str());
  auto reply = static_cast<redisReply*>(cmd);

  if (!reply || (reply->type == REDIS_REPLY_ERROR)) {
    if (reply) {
      freeReplyObject(reply);
    }

    redisFree(ctx);

    throw std::runtime_error("redis auth failed");
  }

  redis_ctx_ = std::shared_ptr<redisContext>(ctx, redisFree);
}

void redis_cache::reconnect() {
  if (redis_ctx_->err) {
    PU_LOG_ERR("redis connection error: {}", redis_ctx_->errstr);
    connect();
  }
}

std::optional<std::string> redis_cache::get(std::string_view key) {
  reconnect();

  auto cmd = redisCommand(redis_ctx_.get(), "GET %s", key.data());
  auto reply = static_cast<redisReply*>(cmd);

  if (!reply) {
    PU_LOG_ERR("redis GET command failed");
    return std::nullopt;
  }

  std::optional<std::string> result;

  if (reply->type == REDIS_REPLY_STRING) {
    PU_LOG_INF("get value ({}) from cache for key ({})", reply->str, key);
    result = reply->str;
  }

  freeReplyObject(reply);
  return result;
}

bool redis_cache::exists(std::string_view key) {
  reconnect();

  auto cmd = redisCommand(redis_ctx_.get(), "EXISTS %s", key.data());
  auto reply = static_cast<redisReply*>(cmd);

  if (!reply) {
    PU_LOG_ERR("redis EXISTS command failed");
    return false;
  }

  bool result = (reply->integer == 1);
  freeReplyObject(reply);

  if (result) {
    PU_LOG_INF("cache hit: {}", key);
  }

  return result;
}

void redis_cache::set(std::string_view key, std::string_view value, const std::uint16_t ttl_sec) {
  reconnect();

  void* cmd = nullptr;

  if (ttl_sec > 0) {
    cmd = redisCommand(redis_ctx_.get(), "SETEX %s %d %s", key.data(), ttl_sec, value.data());
  } else {
    cmd = redisCommand(redis_ctx_.get(), "SET %s %s", key.data(), value.data());
  }

  auto reply = static_cast<redisReply*>(cmd);

  if (!reply) {
    PU_LOG_ERR("redis SET(SETEX) command failed");
  }

  PU_LOG_INF("add to cache [{}:{}]; expires after: {}s", key, value, ttl_sec);

  freeReplyObject(reply);
}

void redis_cache::remove(std::string_view key) {
  reconnect();

  auto cmd = redisCommand(redis_ctx_.get(), "DEL %s", key.data());
  auto reply = static_cast<redisReply*>(cmd);

  if (!reply) {
    PU_LOG_ERR("redis DEL command failed");
  }

  freeReplyObject(reply);
} 

} // namespace prettyurl::infra::caching 