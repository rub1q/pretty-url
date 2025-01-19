#pragma once

#include "prettyurl/core/config/app_config.hpp"

namespace prettyurl {

class application final {
public:
  application(const application&) = delete;
  application(application&&) = delete;

  application& operator=(const application&) = delete;
  application& operator=(application&&) = delete;  

  static void run(const core::config::app_config& cfg);

private:
  application() = default;
};

} // namespace prettyurl