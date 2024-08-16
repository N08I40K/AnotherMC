//
// Created by n08i40k on 14.08.2024.
//

#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H
#include <boost/asio/ip/tcp.hpp>

class transfer_context;

namespace network {
class server_session {
	std::mutex mutex;

	boost::asio::ip::tcp::socket socket;
	std::string                  ip;

	std::vector<std::unique_ptr<transfer_context> > received_packets;

	explicit
	server_session(
		boost::asio::ip::tcp::socket socket);

	void
	receive_header_callback(
		const boost::system::error_code&  error,
		[[maybe_unused]] size_t           bytes_transferred,
		std::unique_ptr<transfer_context> transfer_context);
	void
	receive_data_callback(
		const boost::system::error_code&  error,
		[[maybe_unused]] size_t           bytes_transferred,
		std::unique_ptr<transfer_context> transfer_context);

	void
	send_packet_callback(
		const boost::system::error_code& error,
		[[maybe_unused]] size_t          bytes_transferred);

public:
	static std::unique_ptr<server_session>
	create(
		boost::asio::ip::tcp::socket socket);

	void
	start();

	void
	send(
		transfer_context& transfer_ctx);

	void
	receive();
};
} // network

#endif //SERVER_SESSION_H
