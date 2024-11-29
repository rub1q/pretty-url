#pragma once

namespace prettyurl::core::net::http {

class base_session {
public:
  virtual ~base_session() = default;

  virtual void start() = 0;
  virtual void close() = 0;
};

} // namespace prettyurl::core::net::http 