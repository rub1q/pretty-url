#pragma once

#include "prettyurl/core/net/http/common.hpp"

namespace prettyurl::core::net::http {

class base_request {
public:
  virtual ~base_request() = default;

  [[nodiscard]] virtual http::emethod method() const = 0;
  [[nodiscard]] virtual std::string_view target() const = 0;
  [[nodiscard]] virtual std::string_view header(std::string_view key) const = 0;
  [[nodiscard]] virtual std::string_view body() const = 0;
  [[nodiscard]] virtual bool keep_alive() const = 0;
  [[nodiscard]] virtual unsigned version() const = 0;

  virtual void method(const http::emethod method) = 0;
  virtual void body(std::string_view body) = 0;
  virtual void target(std::string_view target) = 0;
  virtual void header(std::string_view key, std::string_view value) = 0;
  virtual void keep_alive(const bool keep) = 0;
  virtual void version(const unsigned version) = 0;
  virtual void clear() = 0;
};

} // namespace prettyurl::core::net::http