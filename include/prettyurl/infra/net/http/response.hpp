#pragma once

#include "prettyurl/core/net/http/base_response.hpp"
#include "prettyurl/infra/net/http/common.hpp"

namespace prettyurl::infra::net::http {

class response final : public core::net::http::base_response<http_string_response> {
public:
  [[nodiscard]] core::net::http::status status_code() const final;
  [[nodiscard]] std::string_view header(std::string_view key) const final;
  [[nodiscard]] std::string_view body() const final;
  [[nodiscard]] bool keep_alive() const final;
  [[nodiscard]] bool need_eof() const;
  [[nodiscard]] unsigned version() const final;

  void status_code(const core::net::http::status status_code) final;
  void header(std::string_view key, std::string_view value) final;
  void body(std::string_view body) final; 
  void keep_alive(const bool keep) final; 
  void version(const unsigned version) final;
  void prepare_payload() final;
  void clear() final;
};

} // namespace prettyurl::infra::net::http