#pragma once

#include "prettyurl/core/db/db_session_holder.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <concepts>

namespace prettyurl::core::db {

template <typename DBSession>
requires(std::derived_from<DBSession, base_db_session>)
class db_session_manager {
public:
  explicit db_session_manager(const std::size_t pool_size, std::string_view conn_string) {
    if (pool_size == 0u) {
      throw std::invalid_argument("sessions pool size must be > 0");
    }

    if (conn_string.empty()) {
      throw std::invalid_argument("connection string is empty");
    }
    
    for (std::size_t i = 0u; i < pool_size; i++) {
      try {
        session_pool_.emplace(std::make_shared<DBSession>(std::move(conn_string)));

        PU_LOG_INF_TO("console"_logger, "db session connect... [OK]");
        PU_LOG_INF("db session connect... [OK]");
      } catch (const std::exception& e) {
        PU_LOG_ERR_TO("console"_logger, "db session connect... [Fail] ({})", e.what());
        PU_LOG_ERR("db session connect... [Fail] ({})", e.what());
      }
    }

    if (session_pool_.empty()) {
      throw std::runtime_error("failed to initialize db session manager");
    }
  }

  template <typename Rep, typename Period>
  [[nodiscard]] db_session_holder<DBSession> get_session(std::chrono::duration<Rep, Period> timeout) {
    std::unique_lock lk { mtx_ };

    if (!cv_.wait_for(lk, timeout, [this]{ return !session_pool_.empty(); })) {
      throw std::runtime_error("timeout waiting for db session acquire");
    }

    auto session = session_pool_.front();
    session_pool_.pop();

    if (!session->is_connected()) {
      session->reconnect();
    }

    return db_session_holder(std::move(session), *this);
  }

  [[nodiscard]] std::size_t free_sessions_count() const {
    return session_pool_.size();
  }

  void free(std::shared_ptr<DBSession> session) {
    std::unique_lock lk { mtx_ };

    session_pool_.push(std::move(session));
    cv_.notify_one();
  }

private:
  db_session_manager() = default;

private:
  std::mutex mtx_;
  std::condition_variable cv_;

  std::queue<std::shared_ptr<DBSession>> session_pool_;
};

} // namespace prettyurl::core::db