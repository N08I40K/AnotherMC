//
// Created by n08i40k on 14.08.2024.
//

#include "server.h"

#include <print>

#include "server_session.h"

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
			server::accept_callback(ec, std::move(sock));
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
	auto session = server_session::create(std::move(sock));

	sessions.emplace_back(session);
	if (new_connection_handler != nullptr)
		new_connection_handler(session);
}

void
server::thread() {
	std::println("Server thread started!");
	start_accept();
	while (!stop)
		io_context.run();
	std::println("Server thread stopped!");
}

void
server::start_thread() {
	stop      = false;
	io_thread = std::thread(&server::thread, this);
}

void
server::stop_thread() {
	stop = true;
	io_context.stop();
	io_thread.join();
}
} // network
