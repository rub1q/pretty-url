#include "prettyurl/infra/db/auto_inc_id_generator.hpp"

namespace prettyurl::infra::db {

std::uint64_t auto_inc_id_generator::generate() const {
  return repo_->get_last_id() + 1;
}

} // namespace prettyurl::infra::db