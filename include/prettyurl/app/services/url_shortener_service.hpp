#pragma once

#include "prettyurl/core/repositories/url_repository.hpp"

#include <memory>

namespace prettyurl::app::services {

class url_shortener_service final {
public:
  explicit url_shortener_service(std::shared_ptr<core::repositories::url_repository> repo)
    : repo_(std::move(repo)) {
  }
  
  [[nodiscard]] std::string shorten(std::string_view src_url);
  [[nodiscard]] bool is_valid_url(std::string_view src_url);

private:
  std::shared_ptr<core::repositories::url_repository> repo_;
};

} // namespace prettyurl::app::services