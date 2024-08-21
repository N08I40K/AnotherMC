//
// Created by n08i40k on 14.08.2024.
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H
#include <mutex>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace network {
class server_session;

class server {
	boost::asio::io_context        io_context;
	boost::asio::ip::tcp::acceptor acceptor;

	std::function<void(
		std::shared_ptr<server_session>)> new_connection_handler;

	bool        stop{};
	std::thread io_thread;

	std::vector<std::shared_ptr<server_session> > sessions;

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

	[[nodiscard]] const std::vector<std::shared_ptr<server_session> >&
	get_sessions() const { return sessions; }

	void
	set_new_connection_handler(
		const std::function<void(
			std::shared_ptr<server_session>)>& new_connection_handler) {
		this->new_connection_handler = new_connection_handler;
	}
};
} // network

#endif //NETWORK_SERVER_H
