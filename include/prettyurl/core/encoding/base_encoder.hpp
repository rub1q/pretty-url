#pragma once

#include <string>

namespace prettyurl::core::encoding {

struct base_encoder {
  virtual ~base_encoder() = default;

  [[nodiscard]] virtual std::string encode(const std::uint64_t) const = 0;
  [[nodiscard]] virtual std::uint64_t decode(const std::string&) const = 0;
};

} // namespace prettyurl::core::encoding