#pragma once

#include "prettyurl/core/config/db_config.hpp"
#include "prettyurl/core/config/logging_config.hpp"
#include "prettyurl/core/config/server_config.hpp"
#include "prettyurl/core/config/cache_config.hpp"

namespace prettyurl::core::config {

struct app_config final {
  server_config server;
  db_config db;
  cache_config cache;
  logging_config logging;
};

} // namespace prettyurl::core::config