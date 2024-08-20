//
// Created by n08i40k on 14.08.2024.
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H
#include <mutex>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace network {
class server {
	boost::asio::io_context        io_context;
	boost::asio::ip::tcp::acceptor acceptor;

	bool        stop{};
	std::thread io_thread;

	void
	start_accept();

	void
	accept_callback(
		const boost::system::error_code& ec,
		boost::asio::ip::tcp::socket     sock);

	void
	thread();

public:
	server(
		const std::string& host,
		uint16_t           port);

	void
	start_thread();
	void
	stop_thread();
};
} // network

#endif //NETWORK_SERVER_H
