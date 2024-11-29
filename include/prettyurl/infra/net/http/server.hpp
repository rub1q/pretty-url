#pragma once

#include "prettyurl/core/net/http/base_server.hpp"
#include "prettyurl/infra/net/listener.hpp"
#include "prettyurl/infra/net/http/session.hpp"
#include "prettyurl/app/logging/log-inl.hpp"

#include <thread>
#include <vector>
#include <unordered_set>
#include <concepts>

namespace prettyurl::infra::net::http {

template <typename RequestHandler>
class server : public core::net::http::base_server {
public:
  constexpr explicit server(RequestHandler&& handler)
    : request_handler_(std::forward<decltype(handler)>(handler)) {
  }

  void listen_and_serve(const std::uint16_t port) override {
    listen_signals();

    const auto endpoint = tcp::endpoint(net::ip::make_address_v4("0.0.0.0"), port);
    
    std::make_shared<listener>(io_ctx_, endpoint, [this](tcp::socket socket) {
      start_session(std::move(socket));
    })->accept();    
    
    run_thread_pool(threads_count_, [this] {
      const auto thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
      
      app::logging::get("file")->info("run thread {:#x}", thread_id);
      app::logging::get("console")->info("run thread {:#x}", thread_id);

      io_ctx_.run();

      app::logging::get("file")->info("stop thread {:#x}", thread_id);
      app::logging::get("console")->info("stop thread {:#x}", thread_id);
    });
  }

  server& concurrency(const unsigned threads_count) noexcept {
    threads_count_ = threads_count;
    return *this;
  }

  template <typename... Args>
  requires(std::conjunction_v<std::is_same<Args, int>...>)
  server& watch_signals(Args... signals) {
    (signals_.add(signals), ...);
    return *this;
  }

  void stop() override {
    signals_.clear();
    io_ctx_.stop();
  }

private:
  void start_session(tcp::socket socket) {
    std::make_shared<session<RequestHandler>>(std::move(socket), std::move(request_handler_))->start();
  }

  void listen_signals() {
    signals_.async_wait([this] (const sys::error_code& ec, const int signal_number) {
      if (!ec) {
        app::logging::get("file")->info("signal received {}", signal_number);
        app::logging::get("console")->info("signal received {}", signal_number);
        
        stop();
      } else {
        app::logging::get("console")->info("error listen_signals {}", ec.value());
      }
    });
  }

  template <typename Fn>
  void run_thread_pool(const unsigned threads_count, Fn&& func) {
    const auto pool_size = std::min(threads_count, std::thread::hardware_concurrency());

    std::vector<std::jthread> pool;
    pool.reserve(pool_size);

    for (std::size_t i = 0; i < pool_size; i++) {
      pool.emplace_back(std::forward<decltype(func)>(func));
    }
  }

private:
  unsigned threads_count_ { 2u };

  asio::io_context io_ctx_;
  asio::signal_set signals_ { io_ctx_, SIGINT, SIGTERM };

  RequestHandler request_handler_;
};

} // namespace prettyurl::infra::net::http 