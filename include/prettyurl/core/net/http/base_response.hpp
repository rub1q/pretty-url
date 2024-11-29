#pragma once

#include "prettyurl/core/net/http/common.hpp"

namespace prettyurl::core::net::http {

template <typename ResponseT>
class base_response {
public:
  virtual ~base_response() = default;

  [[nodiscard]] virtual http::status status_code() const = 0;
  [[nodiscard]] virtual std::string_view header(std::string_view key) const = 0;
  [[nodiscard]] virtual std::string_view body() const = 0;
  [[nodiscard]] virtual bool keep_alive() const = 0;
  [[nodiscard]] virtual bool need_eof() const = 0;
  [[nodiscard]] virtual unsigned version() const = 0;

  virtual void status_code(const http::status status_code) = 0;
  virtual void header(std::string_view key, std::string_view value) = 0;
  virtual void body(std::string_view body) = 0; 
  virtual void keep_alive(const bool keep) = 0; 
  virtual void version(const unsigned version) = 0;
  virtual void prepare_payload() = 0;
  virtual void clear() = 0;

  const ResponseT& operator*() const noexcept {
    return get();
  }

  ResponseT& operator*() noexcept {
    return get();
  }

  const ResponseT& get() const noexcept {
    return resp_impl_;
  }

  ResponseT& get() noexcept {
    return resp_impl_;
  }

protected:
  ResponseT resp_impl_;
};

} // namespace prettyurl::core::net::http