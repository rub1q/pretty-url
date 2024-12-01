#pragma once

#include "prettyurl/core/net/http/base_request.hpp"
#include "prettyurl/infra/net/http/common.hpp"

namespace prettyurl::infra::net::http {

class request final : public core::net::http::base_request {
public:
  [[nodiscard]] core::net::http::emethod method() const final;
  [[nodiscard]] std::string_view target() const final;
  [[nodiscard]] std::string_view header(std::string_view key) const final;
  [[nodiscard]] std::string_view body() const final;
  [[nodiscard]] bool keep_alive() const final;
  [[nodiscard]] unsigned version() const final;

  void method(const core::net::http::emethod method) final;
  void body(std::string_view body) final;
  void target(std::string_view target) final;
  void header(std::string_view key, std::string_view value) final;
  void keep_alive(const bool keep) final;
  void version(const unsigned version) final;
  void clear() final;

  const http_string_request& get_impl() const noexcept {
    return req_impl_;
  }

  http_string_request& get_impl() noexcept {
    return req_impl_;
  }

private:
  http_string_request req_impl_;
};

} // namespace prettyurl::infra::net::http