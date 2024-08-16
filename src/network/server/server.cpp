//
// Created by n08i40k on 14.08.2024.
//

#include "server.h"

#include <print>

using namespace boost::asio;
using ip::tcp;

namespace network {
server::server(
	const std::string& host,
	const uint16_t     port): acceptor(io_context, tcp::endpoint(ip::address::from_string(host), port)) {}

void
server::start_accept() {
	acceptor.async_accept([this](
	auto ec,
	auto sock) {
			server::accept_callback(ec, sock);
		});
}

void
// ReSharper disable once CppMemberFunctionMayBeStatic
server::accept_callback( // NOLINT(*-convert-member-functions-to-static)
	const boost::system::error_code& ec,
	tcp::socket                      sock) {
	if (ec)
		return;

	std::println("server::accept_callback(): Новое подключение!");
}

void
server::thread() {
	std::println("Server thread started!");
	while (!io_context.stopped())
		io_context.run();
	std::println("Server thread stopped!");
}

void
server::start_thread() { io_thread = std::thread(&server::thread); }

void
server::stop_thread() {
	io_context.stop();
	io_thread.join();
}
} // network
