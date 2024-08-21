//
// Created by n08i40k on 20.08.2024.
//

#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H
#include <memory>
#include <vector>
#include <boost/asio/ip/tcp.hpp>

#include "network/packet_id.h"

namespace network {
class transfer_context;

class client_session {
	using socket_t = boost::asio::ip::tcp::socket;

	bool                    stop{};
	boost::asio::io_context io_context{};
	std::thread             io_thread;

	std::vector<packet_t> received_packets;
	socket_t              socket;

	std::mutex mutex;

	void
	receive_packet();

	void
	receive_packet_header_callback(
		const boost::system::error_code& error,
		[[maybe_unused]] size_t          bytes_transferred,
		packet_t                         packet);

	void
	receive_packet_data_callback(
		const boost::system::error_code& error,
		[[maybe_unused]] size_t          bytes_transferred,
		packet_t                         packet);

	void
	thread();

public:
	explicit
	client_session(
		const std::string& host);
	~client_session();

	void
	start_thread();

	void
	send_packet(
		packet_t&& packet);

	std::vector<packet_t>
	get_received_packets();
};
} // network

#endif //CLIENT_SESSION_H
