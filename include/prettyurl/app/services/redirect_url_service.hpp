#pragma once

#include "prettyurl/core/repositories/url_repository.hpp"
#include "prettyurl/core/caching/base_cache.hpp"

#include <memory>

namespace prettyurl::app::services {

class redirect_url_service final {
public:
  explicit redirect_url_service(std::shared_ptr<core::repositories::url_repository> repo)
    : repo_(std::move(repo))
    , cache_(nullptr) {
  }

  explicit redirect_url_service(std::shared_ptr<core::repositories::url_repository> repo, 
    std::shared_ptr<core::caching::base_cache> cache)
    : repo_(std::move(repo))
    , cache_(std::move(cache)) {
  }

  [[nodiscard]] std::optional<std::string> get_source_url(std::string_view short_url);

private:
  std::shared_ptr<core::repositories::url_repository> repo_;
  std::shared_ptr<core::caching::base_cache> cache_;
};

} // namespace prettyurl::app::services