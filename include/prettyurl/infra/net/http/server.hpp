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

  void listen_and_serve(std::string_view bindaddr, const std::uint16_t port) override {
    try {
      listen_signals();

      const auto endpoint = tcp::endpoint(net::ip::make_address(bindaddr), port);
      
      std::make_shared<listener>(io_ctx_, endpoint, [this](tcp::socket&& socket) {
        start_session(std::move(socket));
      })->accept();

      PU_LOG_INF("server listen on [{}:{}]", bindaddr, port);
      PU_LOG_INF_TO("console"_logger, "server listen on [{}:{}]", bindaddr, port);
      
      run_thread_pool(threads_count_, [this] {
        const auto thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
        
        PU_LOG_INF("run thread {:#x}", thread_id);
        PU_LOG_INF_TO("console"_logger, "run thread {:#x}", thread_id);

        io_ctx_.run();

        PU_LOG_INF("stop thread {:#x}", thread_id);
        PU_LOG_INF_TO("console"_logger, "stop thread {:#x}", thread_id);
      });
    } catch (const std::exception& e) {
      PU_LOG_ERR("an error occured while serving connection ({})", e.what());
      return stop();
    } catch (...) {
      PU_LOG_ERR("an unknown error occured while serving connection");
      return stop();
    }
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
  void start_session(tcp::socket&& socket) {
    auto new_session = std::make_shared<session<RequestHandler>>(io_ctx_, std::move(socket), request_handler_);

    // TODO: set timeout values from config
    new_session->read_timeout(std::chrono::minutes(5));
    new_session->write_timeout(std::chrono::minutes(5));
    new_session->start();
  }

  void listen_signals() {
    signals_.async_wait([this](const sys::error_code& ec, const int signal_number) {
      if (ec) {
        PU_LOG_ERR("an error occured at listen_signals() {}", ec.value());
        return;
      }

      PU_LOG_INF("signal received {}", signal_number);
      PU_LOG_INF_TO("console"_logger, "signal received {}", signal_number);
      
      stop();
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