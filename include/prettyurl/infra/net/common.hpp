#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>

namespace prettyurl::infra::net {

namespace asio = boost::asio;
namespace net = boost::asio;
namespace sys = boost::system;

using tcp = net::ip::tcp;

} // namespace prettyurl::infra::net