#pragma once

#include "prettyurl/core/repositories/url_repository.hpp"
#include "prettyurl/core/db/db_session_manager.hpp"

#include "prettyurl/infra/db/pg_session.hpp"

namespace prettyurl::infra::db::repository {

class pg_url_repository_impl final : public core::repositories::url_repository {
public:
  explicit pg_url_repository_impl(core::db::db_session_manager<pg_session>& dbsm)
    : dbsm_(dbsm) {
  }

  [[nodiscard]] std::uint64_t get_last_id() final;
    
  [[nodiscard]] std::optional<std::string> get_long_url(std::string_view short_url) final;
  [[nodiscard]] std::optional<std::string> get_short_url(std::string_view long_url) final;
  
  void add_short_url(std::string_view short_url, std::string_view long_url) final;

private:
  core::db::db_session_manager<pg_session>& dbsm_;
};

} // namespace prettyurl::infra::db::repository