#include "prettyurl/core/encoding/base62_encoder.hpp"

#include <stdexcept>

namespace prettyurl::core::encoding {

std::string base62_encoder::encode(const std::uint64_t value) const {
  if (value == 0) {
    return "0";
  }

  auto val = value;
  std::string result;

  while (val > 0) {
    result = charset[val % 62] + result;
    val /= 62;
  }

  return result;
}

std::uint64_t base62_encoder::decode(const std::string& src) const {
  std::uint64_t result = 0u;

  for (auto c : src) {
    result = result * 62 + to_int(c);
  }

  return result;
}

int base62_encoder::to_int(const char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }

  if (c >= 'A' && c <= 'Z') {
    return c - 'A' + 10;
  }
  
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 36;
  }

  throw std::invalid_argument("invalid character");
}

} // namespace prettyurl::core::utility