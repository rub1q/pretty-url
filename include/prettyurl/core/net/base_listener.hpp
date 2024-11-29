#pragma once

namespace prettyurl::core::net {

class base_listener {
public:
  virtual ~base_listener() = default;

  virtual void accept() = 0;
  virtual void close() = 0;
};

} // namespace prettyurl::core::net