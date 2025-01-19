#include "prettyurl/infra/net/http/request.hpp"

namespace prettyurl::infra::net::http {

core::net::http::emethod request::method() const {
  return core::net::http::method::from_string(req_impl_.method_string());
}

std::string_view request::target() const {
  return req_impl_.target();
}

std::string_view request::header(std::string_view key) const {
  return req_impl_[key];
}

std::string_view request::body() const {
  return req_impl_.body();
}

bool request::keep_alive() const {
  return req_impl_.keep_alive();
}

unsigned request::version() const {
  return req_impl_.version();
}

void request::method(const core::net::http::emethod method) {
  req_impl_.method_string(core::net::http::method::to_string(method));
}

void request::body(std::string_view body) {
  req_impl_.body() = std::move(body);
}

void request::target(std::string_view target) {
  req_impl_.target(target);
}

void request::header(std::string_view key, std::string_view value) {
  req_impl_.set(key, value);
}

void request::keep_alive(const bool keep) {
  req_impl_.keep_alive(keep);
}

void request::version(const unsigned version) {
  req_impl_.version(version);
}

void request::clear() {
  req_impl_.clear();
  req_impl_ = {};
}

const request::vars_map& request::vars() const {
  return vars_;
}

void request::vars(const vars_map& vars) {
  vars_.clear();
  vars_.reserve(vars.size());
  
  vars_ = vars;
}

} // namespace prettyurl::infra::net::http