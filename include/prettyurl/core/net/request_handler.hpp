#pragma once

#include <functional>

namespace prettyurl::core::net {

template <typename Request, typename Response>
class request_handler {
public:
  virtual ~request_handler() = default;

  [[nodiscard]] virtual Response operator()(Request&&) = 0;
};

template <typename Request, typename Response>
class request_handler_func final : public request_handler<Request, Response> {
public:
  using HandlerFunc = std::function<Response(Request&&)>;

  request_handler_func(HandlerFunc handler)
    : handler_(std::move(handler)) {
  }

  request_handler_func(const request_handler_func&) = default;
  request_handler_func(request_handler_func&&) = default;

  request_handler_func& operator=(request_handler_func&&) = default;
  request_handler_func& operator=(const request_handler_func&) = default;

  Response operator()(Request&& req) final {
    return handler_(std::forward<decltype(req)>(req));
  }

private:
  HandlerFunc handler_;
};

} // namespace prettyurl::core::net