#pragma once

#include "prettyurl/core/repositories/url_repository.hpp"

namespace prettyurl::infra::db::repository {

struct pg_url_repository_impl final : public core::repositories::url_repository {
  [[nodiscard]] std::optional<std::string> get_long_url(std::string_view short_url) final;
  
  void add_short_url(std::string_view short_url, std::string_view long_url) final;  
};

} // namespace prettyurl::infra::db::repository