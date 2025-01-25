#pragma once

#include <string>
#include <type_traits>
#include <algorithm>
#include <charconv>
#include <stdexcept>

namespace prettyurl::core::utility {

constexpr inline void to_lower(std::string& value) {
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);
}

constexpr inline void to_upper(std::string& value) {
  std::transform(value.begin(), value.end(), value.begin(), ::toupper);
}

template <typename T>
[[nodiscard]] constexpr inline std::string to_string(T&& value) {
  if constexpr (std::is_same_v<std::decay_t<T>, char>) {
    return std::string(1, std::forward<T>(value));
  } else if constexpr (std::is_integral_v<std::decay_t<T>> || 
                       std::is_floating_point_v<std::decay_t<T>>) {
    return std::to_string(std::forward<T>(value));
  } else if constexpr (std::is_base_of_v<std::decay_t<T>, std::basic_string<char>> || 
                       std::is_same_v<std::decay_t<T>, const char*>) {
    return value;
  } else if constexpr (std::is_base_of_v<std::decay_t<T>, std::basic_string_view<char>>) {
    return value.data();
  } else {
    static_assert(!std::is_same_v<T, T>, "unsupported data type");
  }
}

template <typename FirstArg, 
          typename SecondArg, 
          typename... Args>
[[nodiscard]] constexpr inline std::string concat(FirstArg&& first, SecondArg&& second, Args&&... args) {
  std::string out = to_string(std::forward<FirstArg>(first)) + " " +
    to_string(std::forward<SecondArg>(second));

  if constexpr (sizeof... (args) > 0) {
    ((out += " " + to_string(std::forward<Args>(args))), ...);
  }

  return out;
}

template <typename T>
[[nodiscard]] constexpr inline T string_to(const std::string& value) {
  if constexpr (std::is_base_of_v<T, std::basic_string<char>>) {
    return value;
  } else if constexpr (std::is_same_v<T, bool>) {
    auto str = value;
    to_lower(str);
    
    if (str == "1" || str == "true") {
      return true;
    } else if (str == "0" || str == "false") {
      return false;
    }

    throw std::runtime_error(concat("invalid conversion '", value, "' to boolean type"));
  } else if constexpr (std::is_same_v<T, char>) {
    if (value.length() > 1) {
      throw std::invalid_argument(concat("invalid conversion '", value, "' to char type"));
    }

    return value[0];
  } else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
    T result;

    auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.length(), result);

    if (ec != std::errc()) {
      throw std::runtime_error(concat("invalid conversion '", value, "' to integral or float type"));
    }

    return result;
  } else {
    static_assert(!std::is_same_v<T, T>, "unsupported data type");
  }

  return {};
}

} // namespace prettyurl::core::utility