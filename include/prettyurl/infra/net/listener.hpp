#pragma once

#include "prettyurl/core/net/base_listener.hpp"
#include "prettyurl/infra/net/common.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

#include <memory>
#include <functional>

namespace prettyurl::infra::net {

class listener final : public core::net::base_listener, public std::enable_shared_from_this<listener> {
public:
  using Callback = std::function<void(tcp::socket)>;

  explicit listener(asio::io_context& io_ctx, const tcp::endpoint& endpoint, Callback&& callback)
    : io_ctx_(io_ctx)
    , acceptor_(io_ctx_, endpoint)
    , callback_(std::move(callback)) {
    
    acceptor_.set_option(net::socket_base::reuse_address(true));
    acceptor_.listen(net::socket_base::max_listen_connections);
  }

  void accept() override {
    return do_accept();
  }

  void close() override {
    return do_close();
  }

private:
  void do_accept() {
    acceptor_.async_accept(std::bind_front(&listener::on_accept, this->shared_from_this()));
  }

  void do_close() {
    acceptor_.close();
  }

  void on_accept(sys::error_code ec, tcp::socket socket) {
    if (ec) {
      return app::logging::error("http: an error occured during accepting new connection (code: {}; message: {})", ec.value(), ec.message());
    }

    callback_(std::move(socket));
    accept();  
  }

private:
  asio::io_context& io_ctx_;
  tcp::acceptor acceptor_;

  Callback callback_;
};

} // namespace prettyurl::infra::net