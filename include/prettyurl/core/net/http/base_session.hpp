#pragma once

namespace prettyurl::core::net::http {

template <typename RequestT, typename ResponseT>
class base_session {
public:
  virtual ~base_session() = default;

  virtual void start() = 0;
  virtual void close() = 0;

  virtual void read_request(RequestT&& req) = 0;
  virtual void write_response(ResponseT&& resp) = 0;
};

} // namespace prettyurl::core::net::http 