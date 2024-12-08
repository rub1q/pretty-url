#pragma once

#include "prettyurl/core/repositories/url_repository.hpp"

#include <memory>

namespace prettyurl::app::services {

class redirect_url_service final {
public:
  explicit redirect_url_service(std::shared_ptr<core::repositories::url_repository> repo)
    : repo_(std::move(repo)) {
  }

  [[nodiscard]] std::optional<std::string> get_source_url(std::string_view short_url);

private:
  std::shared_ptr<core::repositories::url_repository> repo_;
};

} // namespace prettyurl::app::services