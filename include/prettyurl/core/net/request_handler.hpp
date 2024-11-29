#pragma once

namespace prettyurl::core::net {

template <typename Request, typename Response>
class request_handler {
public:
  virtual ~request_handler() = default;

  [[nodiscard]] virtual Response operator()(Request&&) = 0;
};

} // namespace prettyurl::core::net