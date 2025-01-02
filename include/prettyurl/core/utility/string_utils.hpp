#pragma once

#include <string>
#include <type_traits>
#include <algorithm>
#include <charconv>
#include <stdexcept>

namespace prettyurl::core::utility {

inline void to_lower(std::string& value) {
  std::transform(value.begin(), value.end(), value.begin(), 
    [](unsigned char c) { return std::tolower(c); });
}

template <typename T>
[[nodiscard]] inline std::string to_string(T&& value) {
  if constexpr (std::is_integral_v<std::decay_t<T>> || 
                std::is_floating_point_v<std::decay_t<T>>) {
    return std::to_string(value);
  } else if constexpr (std::is_base_of_v<std::decay_t<T>, std::basic_string<char>> || 
                        std::is_base_of_v<std::decay_t<T>, std::basic_string_view<char>> ||
                        std::is_same_v<std::decay_t<T>, const char*>) {
    return value.data();
  } else {
    static_assert(!std::is_same_v<T, T>, "unsupported data type");
  }
}

template <typename T>
[[nodiscard]] inline T string_to(const std::string& value) {
  if constexpr (std::is_same_v<T, std::string>) {
    return value;
  } else if constexpr (std::is_same_v<T, bool>) {
    auto str = value;
    to_lower(str);
    
    if (str == "1" || str == "true") {
      return true;
    } else if (str == "0" || str == "false") {
      return false;
    }

    throw std::runtime_error("invalid conversation '" + value + "' to boolean type");
  } else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
    T result;

    auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.length(), result);

    if (ec != std::errc()) {
      throw std::runtime_error("invalid conversation '" + value + "' to integral or float type");
    }

    return result;
  } 
}

} // namespace prettyurl::core::utility