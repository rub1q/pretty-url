#include "prettyurl/infra/net/http/response.hpp"

namespace prettyurl::infra::net::http {

core::net::http::estatus response::status_code() const {
  return static_cast<core::net::http::estatus>(resp_impl_.result_int());
}

std::string_view response::header(std::string_view key) const {
  return resp_impl_[key];
}

std::string_view response::body() const {
  return resp_impl_.body();
}

bool response::keep_alive() const {
  return resp_impl_.keep_alive();
}

bool response::need_eof() const {
  return resp_impl_.need_eof();
}

unsigned response::version() const {
  return resp_impl_.version();
}

void response::status_code(const core::net::http::estatus status_code) {
  resp_impl_.result(static_cast<int>(status_code));
}

void response::header(std::string_view key, std::string_view value) {
  resp_impl_.set(key, value);
}

void response::body(std::string_view body) {
  resp_impl_.body() = std::move(body);
}

void response::keep_alive(const bool keep) {
  resp_impl_.keep_alive(keep);
} 

void response::version(const unsigned version) {
  resp_impl_.version(version);
}

void response::prepare_payload() {
  resp_impl_.prepare_payload();
}

void response::clear() {
  resp_impl_.clear();
}

const http_string_response& response::get_impl() const noexcept {
  return resp_impl_;
}

http_string_response& response::get_impl() noexcept {
  return resp_impl_;
}

} // namespace prettyurl::infra::net::http