#ifdef HAS_STD_FORMAT_SUPPORT
  #include <format>

  #define _make_format_args(...)  std::make_format_args(__VA_ARGS__)
  #define _vformat(fmt, ...)      std::vformat(fmt, _make_format_args(__VA_ARGS__));
#else
  #include "fmt/core.h"

  #define _make_format_args(...)  fmt::make_format_args(__VA_ARGS__)
  #define _vformat(fmt, ...)      fmt::vformat(fmt, _make_format_args(__VA_ARGS__));
#endif

namespace utils {

template <typename... Args>
inline std::string format(const char* fmt, Args&&... args) {
  return _vformat(fmt, args...);
}

} // namespace utils