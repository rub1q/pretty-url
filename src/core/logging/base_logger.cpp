#include "prettyurl/core/logging/base_logger.hpp"

namespace prettyurl::core::logging {

void base_logger::set_log_level(const level::elevel level) noexcept {
  log_lvl_ = level;
}

level::elevel base_logger::get_log_level() const noexcept {
  return log_lvl_;
}

std::string_view base_logger::name() const noexcept {
  return name_;
}  

} // namespace prettyurl::core::logging