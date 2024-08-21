//
// Created by n08i40k on 14.08.2024.
//

#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H
#include <boost/asio/ip/tcp.hpp>

#include "network/packet_id.h"

namespace network {
class transfer_context;

class server_session {
	std::mutex mutex;

	boost::asio::ip::tcp::socket socket;
	std::string                  ip;

	std::vector<packet_t> received_packets;

	explicit
	server_session(
		boost::asio::ip::tcp::socket socket);

	void
	receive_header_callback(
		const boost::system::error_code& error,
		[[maybe_unused]] size_t          bytes_transferred,
		packet_t                         packet);
	void
	receive_data_callback(
		const boost::system::error_code& error,
		[[maybe_unused]] size_t          bytes_transferred,
		packet_t                         transfer_context);

	void
	send_packet_callback(
		const boost::system::error_code& error,
		[[maybe_unused]] size_t          bytes_transferred);

	void
	receive();

public:
	static std::shared_ptr<server_session>
	create(
		boost::asio::ip::tcp::socket socket);

	void
	start();

	void
	send(
		transfer_context& transfer_ctx);

	std::vector<packet_t>
	get_received_packets();
};
} // network

#endif //SERVER_SESSION_H
