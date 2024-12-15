#pragma once

#include "prettyurl/core/id/base_id_generator.hpp"
#include "prettyurl/core/repositories/url_repository.hpp"

#include <memory>

namespace prettyurl::infra::db {

class auto_inc_id_generator final : public core::id::base_id_generator {
public:
  explicit auto_inc_id_generator(std::shared_ptr<core::repositories::url_repository> repo)
    : repo_(std::move(repo)) {
  }

  [[nodiscard]] std::uint64_t generate() const final;

private:
  auto_inc_id_generator() = default;

private:
  std::shared_ptr<core::repositories::url_repository> repo_;
};

} // namespace prettyurl::infra::db 