#pragma once

#include "prettyurl/core/encoding/base_encoder.hpp"
#include <string>

namespace prettyurl::core::encoding {

class base62_encoder final : public base_encoder {
public:
  [[nodiscard]] std::string encode(const std::uint64_t value) const final;
  [[nodiscard]] std::uint64_t decode(const std::string& src) const final;

private:
  [[nodiscard]] static int to_int(const char c);

private:
  static constexpr char charset[]{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
};

} // namespace prettyurl::core::encoding