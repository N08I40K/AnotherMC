//
// Created by n08i40k on 14.08.2024.
//

#include "server_session.h"

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include "network/transfer_context.h"

using namespace boost::asio;
using ip::tcp;

namespace network {
server_session::server_session(
	tcp::socket socket): socket(std::move(socket)) {}

void
server_session::receive_header_callback(
	const boost::system::error_code& error,
	[[maybe_unused]] size_t          bytes_transferred,
	packet_t                         packet) {
	if (error)
		throw boost::system::system_error(error);

	auto& receive_buffer = packet->get_data_for_receive();

	auto callback = [this, packet_raw = packet.release()](
		auto cb_error,
		auto cb_bytes_transferred) {
		this->receive_data_callback(cb_error, cb_bytes_transferred, packet_t(packet_raw));
	};
	async_read(
		socket,
		buffer(receive_buffer.data(), receive_buffer.size()),
		transfer_all(),
		callback
	);
}

void
server_session::receive_data_callback(
	const boost::system::error_code& error,
	[[maybe_unused]] size_t          bytes_transferred,
	packet_t                         transfer_context) {
	if (error)
		throw boost::system::system_error(error);

	//
	{
		std::lock_guard lock(mutex);
		received_packets.emplace_back(std::move(transfer_context));
	}

	receive();
}

void
// ReSharper disable once CppMemberFunctionMayBeStatic
server_session::send_packet_callback( // NOLINT(*-convert-member-functions-to-static)
	const boost::system::error_code& error,
	[[maybe_unused]] size_t          bytes_transferred) {
	if (error)
		throw boost::system::system_error(error);
}

std::shared_ptr<server_session>
server_session::create(
	tcp::socket socket) { return std::shared_ptr<server_session>(new server_session(std::move(socket))); }

void
server_session::start() { receive(); }

void
server_session::send(
	transfer_context& transfer_ctx) {
	auto data = transfer_ctx.get_data_for_send();
	async_write(socket,
	            buffer(data.data(), data.size()),
	            transfer_all(),
	            [this](
            auto error,
            auto bytes_transferred) {
		            this->send_packet_callback(error, bytes_transferred);
	            });
}

std::vector<packet_t>
server_session::get_received_packets() {
	std::lock_guard lock(mutex);
	return std::move(received_packets);
}

void
server_session::receive() {
	auto packet = transfer_context::create_receive();

	size_t& size_for_receive = packet->get_size_for_receive();
	async_read(socket,
	           buffer(&size_for_receive, sizeof(size_for_receive)),
	           transfer_all(),
	           [this, packet = std::move(packet)](
           auto error,
           auto bytes_transferred) mutable {
		           this->receive_header_callback(error, bytes_transferred, std::move(packet));
	           });
}
} // network
