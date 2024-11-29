#pragma once

#include "prettyurl/core/net/http/common.hpp"

namespace prettyurl::core::net::http {

template <typename RequestT>
class base_request {
public:
  virtual ~base_request() = default;

  [[nodiscard]] virtual http::method::emethod method() const = 0;
  [[nodiscard]] virtual std::string_view target() const = 0;
  [[nodiscard]] virtual std::string_view header(std::string_view key) const = 0;
  [[nodiscard]] virtual std::string_view body() const = 0;
  [[nodiscard]] virtual bool keep_alive() const = 0;
  [[nodiscard]] virtual unsigned version() const = 0;

  virtual void method(const http::method::emethod method) = 0;
  virtual void body(std::string_view body) = 0;
  virtual void target(std::string_view target) = 0;
  virtual void header(std::string_view key, std::string_view value) = 0;
  virtual void keep_alive(const bool keep) = 0;
  virtual void version(const unsigned version) = 0;
  virtual void clear() = 0;

  const RequestT& operator*() const noexcept {
    return get();
  }

  RequestT& operator*() noexcept {
    return get();
  }

  const RequestT& get() const noexcept {
    return req_impl_;
  }

  RequestT& get() noexcept {
    return req_impl_;
  }

protected:
  RequestT req_impl_;
};

} // namespace prettyurl::core::net::http