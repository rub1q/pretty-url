#pragma once

#include "prettyurl/infra/net/http/common.hpp"
#include "prettyurl/infra/net/http/request.hpp"
#include "prettyurl/infra/net/http/response.hpp"
#include "prettyurl/core/net/http/base_session.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

#include <memory>
#include <chrono>

namespace prettyurl::infra::net::http {

using namespace std::literals;

template <typename RequestHandler>
class session final : public core::net::http::base_session<request, response>, public std::enable_shared_from_this<session<RequestHandler>> {
public:
  constexpr explicit session(asio::io_context& io_ctx, tcp::socket&& socket, const RequestHandler& handler)
    : io_ctx_(io_ctx)
    , socket_(std::move(socket))
    , request_handler_(handler) {
  }

  void start() final {
    PU_LOG_INF("start a new session");
    do_read();
  }

  void close() final {
    socket_.shutdown(net::socket_base::shutdown_both);
    socket_.close();

    PU_LOG_INF("session successfully closed");
  }

  template <typename Rep, typename Period>
  void read_timeout(const std::chrono::duration<Rep, Period>& value) {
    read_timeout_value_ = value;
  }

  template <typename Rep, typename Period>
  void write_timeout(const std::chrono::duration<Rep, Period>& value) {
    write_timeout_value_ = value;
  }

private:
  void do_read() {
    request_.clear();

    set_timeout(read_timeout_value_);
  
    http::async_read(socket_, buf_, 
      request_.get_impl(), 
      asio::bind_executor(strand_, std::bind_front(&session::on_read, self_ptr())));
  }

  void read_request(request&& req) final {
    auto resp = request_handler_(std::move(req));
    write_response(std::move(resp));
  }

  void write_response(response&& resp) final {
    auto sp_resp = std::make_shared<response>(std::move(resp));

    set_timeout(write_timeout_value_);
    
    http::async_write(socket_, sp_resp->get_impl(), 
      [sp_resp, self = self_ptr()](sys::error_code ec, std::size_t bytes_written) {
        self->on_write(sp_resp->need_eof(), ec, bytes_written);
    });
  }

  void on_read(sys::error_code ec, [[maybe_unused]] std::size_t bytes_read) {
    cancel_timeout();
    
    if (ec) {
      PU_LOG_ERR("an error occured while reading data (code: {}; message: {})", ec.value(), ec.message());
      return;
    }

    read_request(std::move(request_));
  }

  void on_write(bool close_socket, sys::error_code ec, [[maybe_unused]] std::size_t bytes_written) {
    cancel_timeout();
    
    if (ec) {
      PU_LOG_ERR("an error occured while writing data (code: {}; message: {})", ec.value(), ec.message());
      return;
    }
    
    if (close_socket) {
      return close();
    }

    do_read();
  }

  template <typename Rep, typename Period>
  void set_timeout(const std::chrono::duration<Rep, Period>& dur) {
    timeout_timer_.expires_after(dur);
    timeout_timer_.async_wait(asio::bind_executor(strand_, [self = self_ptr()](sys::error_code ec) {
      if (!ec) {
        PU_LOG_WRN("operation timeout. closing session...");
        self->close();
      }
    }));
  }

  void cancel_timeout() {
    timeout_timer_.cancel();
  }

  std::shared_ptr<session> self_ptr() {
    return this->shared_from_this();
  }

private:
  using TimeDuration = std::chrono::milliseconds;

  TimeDuration read_timeout_value_ { 5s };
  TimeDuration write_timeout_value_ { 5s };

  asio::io_context& io_ctx_;
  tcp::socket socket_;

  flat_buffer buf_;
  request request_;

  asio::steady_timer timeout_timer_ { io_ctx_ };
  asio::strand<asio::io_context::executor_type> strand_ { asio::make_strand(io_ctx_) };

  RequestHandler request_handler_;
};

} // namespace prettyurl::infra::net::http