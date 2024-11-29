#pragma once

#include "prettyurl/core/logging/base_logger.hpp"

namespace prettyurl::core::logging::loggers {

class null_logger final : public base_logger {
public:
  null_logger() 
    : base_logger("null") {  
  }

private:
  void do_append([[maybe_unused]] const level::elevel level, [[maybe_unused]] std::string_view text) final {
    return;
  }
};

} // namespace prettyurl::core::logging::loggers