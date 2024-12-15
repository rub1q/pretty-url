#pragma once

#include <cstdint>

namespace prettyurl::core::id {

struct base_id_generator {
  virtual ~base_id_generator() = default;

  [[nodiscard]] virtual std::uint64_t generate() const = 0;
};

} // namespace prettyurl::core::id