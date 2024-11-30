#pragma once

#include <cstdint>
#include <string>
#include <algorithm>

namespace prettyurl::core::logging {
namespace level {

enum elevel : std::uint8_t {
  none,
  trace,
  debug,
  info,
  warning,
  error,
  fatal
};

[[nodiscard]] constexpr inline const char* to_string(const elevel level) noexcept {
  switch (level) {
    case elevel::trace :
      return "TRACE";
    case elevel::debug :
      return "DEBUG";
    case elevel::info :
      return "INFO";
    case elevel::warning :
      return "WARNING";
    case elevel::error :
      return "ERROR";
    case elevel::fatal :
      return "FATAL";

    default : 
      return "NONE";
  }
}

[[nodiscard]] constexpr inline const char* to_short_string(const elevel level) noexcept {
  switch (level) {
    case elevel::trace :
      return "TRC";
    case elevel::debug :
      return "DBG";
    case elevel::info :
      return "INF";
    case elevel::warning :
      return "WRN";
    case elevel::error :
      return "ERR";
    case elevel::fatal :
      return "FTL";

    default : 
      return "NONE";
  }
}

[[nodiscard]] constexpr inline elevel from_string(std::string_view str_level) {
  std::string lvl(str_level);

  std::transform(lvl.begin(), lvl.end(), lvl.begin(), 
                 [](unsigned char c) { return std::tolower(c); });

  if (lvl == "trace" || lvl == "trc") {
    return elevel::trace;
  } else if (lvl == "debug" || lvl == "dbg") {
    return elevel::debug;
  } else if (lvl == "info" || lvl == "inf") {
    return elevel::info;
  } else if (lvl == "warning" || lvl == "wrn" || lvl == "warn") {
    return elevel::warning;
  } else if (lvl == "error" || lvl == "err") {
    return elevel::error;
  } else if (lvl == "fatal" || lvl == "ftl") {
    return elevel::fatal;
  }

  return elevel::none;
}
} // namespace level

class source_loc final {
public:  
  source_loc() = default;

  static consteval source_loc current() noexcept {
    source_loc loc;

    loc.line_num_ = __LINE__;
    loc.file_name_ = __FILE__;
    loc.func_name_ = __FUNCTION__;

    return loc;
  }

private:
  int line_num_ { 0 };

  const char* file_name_ { nullptr };
  const char* func_name_ { nullptr };
};

} // namespace prettyurl::core::logging