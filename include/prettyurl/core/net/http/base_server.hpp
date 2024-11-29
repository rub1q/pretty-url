#pragma once

#include <cstdint>

namespace prettyurl::core::net::http {

class base_server {
public:
  virtual ~base_server() = default;

  virtual void listen_and_serve(std::string_view bindaddr, const std::uint16_t port) = 0;
  virtual void stop() = 0;
};

} // namespace prettyurl::core::net::http