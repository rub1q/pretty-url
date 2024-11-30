#pragma once

#include "prettyurl/infra/net/http/common.hpp"
#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"
#include "prettyurl/core/net/http/base_session.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

#include <memory>

namespace prettyurl::infra::net::http {

template <typename RequestHandler>
class session final : public core::net::http::base_session, public std::enable_shared_from_this<session<RequestHandler>> {
public:
  constexpr explicit session(tcp::socket socket, RequestHandler&& handler)
    : socket_(std::move(socket))
    , request_handler_(std::forward<decltype(handler)>(handler)) {
  }

  void start() final {
    do_read();
  }

  void close() final {
    socket_.shutdown(net::socket_base::shutdown_both);
  }  

private:
  void do_read() {
    request_.clear();

    // TODO: add timeouts
  
    http::async_read(socket_, buf_, 
      *request_, std::bind_front(&session::on_read, this->shared_from_this()));
  }

  void send_response(response&& resp) {
    auto sp_resp = std::make_shared<response>(std::move(resp));
    
    http::async_write(socket_, (*sp_resp).get(), 
      [sp_resp, self = this->shared_from_this()](sys::error_code ec, std::size_t bytes_written) {
        self->on_write(sp_resp->need_eof(), ec, bytes_written);
    });
  }

  void handle_request(request&& req) {
    auto resp = request_handler_(std::move(req));
    send_response(std::move(resp));
  }

  void on_read(sys::error_code ec, [[maybe_unused]] std::size_t bytes_read) {
    if (ec) {
      PU_LOG_ERR("http: an error occured while reading data (code: {}; message: {})", ec.value(), ec.message());
      return;
    }

    handle_request(std::move(request_));
  }

  void on_write(bool close_socket, sys::error_code ec, [[maybe_unused]] std::size_t bytes_written) {
    if (ec) {
      PU_LOG_ERR("http: an error occured while writing data (code: {}; message: {})", ec.value(), ec.message());
      return;
    }
    
    if (close_socket) {
      return close();
    }

    do_read();
  }

private:
  tcp::socket socket_;

  flat_buffer buf_;
  request request_;

  RequestHandler request_handler_;
};

} // namespace prettyurl::infra::net::http