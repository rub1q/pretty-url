#pragma once

#include "prettyurl/core/repositories/url_repository.hpp"
#include "prettyurl/core/encoding/base_encoder.hpp"
#include "prettyurl/core/id/base_id_generator.hpp"
#include "prettyurl/core/caching/base_cache.hpp"

#include <memory>

namespace prettyurl::app::services {

class url_shortener_service final {
public:
  explicit url_shortener_service(std::shared_ptr<core::repositories::url_repository> repo, 
    std::shared_ptr<core::encoding::base_encoder> encoder,
    std::shared_ptr<core::id::base_id_generator> id_generator)
    : repo_(std::move(repo))
    , cache_(nullptr)
    , encoder_(std::move(encoder))
    , id_generator_(std::move(id_generator)) {
  }

  explicit url_shortener_service(std::shared_ptr<core::repositories::url_repository> repo, 
    std::shared_ptr<core::caching::base_cache> cache,
    std::shared_ptr<core::encoding::base_encoder> encoder,
    std::shared_ptr<core::id::base_id_generator> id_generator)
    : repo_(std::move(repo))
    , cache_(std::move(cache))
    , encoder_(std::move(encoder))
    , id_generator_(std::move(id_generator)) {
  }
  
  [[nodiscard]] std::string shorten(std::string_view src_url);
  [[nodiscard]] bool is_valid_url(std::string_view src_url);

private:
  std::shared_ptr<core::repositories::url_repository> repo_;
  std::shared_ptr<core::caching::base_cache> cache_;
  std::shared_ptr<core::encoding::base_encoder> encoder_;
  std::shared_ptr<core::id::base_id_generator> id_generator_;
};

} // namespace prettyurl::app::services