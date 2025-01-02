#pragma once

#include <memory>
#include <concepts>

namespace prettyurl::core::db {

class base_db_session;

template <typename DBSession>
requires(std::derived_from<DBSession, base_db_session>)
class db_session_manager;

template <typename DBSession>
requires(std::derived_from<DBSession, base_db_session>)
class db_session_holder {
public:
  explicit db_session_holder(std::shared_ptr<DBSession> session, db_session_manager<DBSession>& session_manager)
    : session_(std::move(session))
    , session_manager_(session_manager) {
  }

  ~db_session_holder() {
    session_manager_.free(session_);
  }

  [[nodiscard]] DBSession& operator*() const {
    return *session_;
  }

  [[nodiscard]] DBSession* operator->() const {
    return session_.get();
  }

  void release() {
    session_manager_.free(session_);
  }

private:
  db_session_holder() = default;

private:
  std::shared_ptr<DBSession> session_;
  db_session_manager<DBSession>& session_manager_;
};

} // namespace prettyurl::core::db