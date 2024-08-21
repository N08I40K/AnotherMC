//
// Created by n08i40k on 20.08.2024.
//

#include "client_session.h"

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include "network/transfer_context.h"

using namespace boost::asio;
using ip::tcp;

namespace network {
void
client_session::receive_packet() {
	auto  packet = transfer_context::create_receive();
	auto& size   = packet->get_size_for_receive();

	auto callback = [this, packet_raw = packet.release()](
		auto ec_cb,
		auto bytes_transferred_cb) {
		this->receive_packet_header_callback(ec_cb, bytes_transferred_cb, packet_t(packet_raw));
	};

	async_read(socket, buffer(&size, sizeof(size)), transfer_all(), callback);
}

void
client_session::receive_packet_header_callback(
	const boost::system::error_code& error,
	[[maybe_unused]] size_t          bytes_transferred,
	packet_t                         packet) {
	if (error)
		throw boost::system::system_error(error);

	auto& data = packet->get_data_for_receive();

	auto callback = [this, packet_raw = packet.release()](
		auto ec_cb,
		auto bytes_transferred_cb) {
		this->receive_packet_data_callback(ec_cb, bytes_transferred_cb, packet_t(packet_raw));
	};
	async_read(socket, buffer(data.data(), data.size()), transfer_all(), callback);
}

void
client_session::receive_packet_data_callback(
	const boost::system::error_code& error,
	[[maybe_unused]] size_t          bytes_transferred,
	packet_t                         packet) {
	if (error)
		throw boost::system::system_error(error);

	std::lock_guard lock(mutex);
	received_packets.emplace_back(std::move(packet));
}

void
client_session::thread() {
	receive_packet();

	while (!stop)
		io_context.run();
}

client_session::client_session(
	const std::string& host)
	: socket(io_context) {
	std::string hostname;
	std::string port;

	if (!host.contains(":")) {
		hostname = host;
		port     = "20512";
	}
	else {
		const auto del_idx = host.find(':');
		hostname           = host.substr(0, del_idx);
		port               = host.substr(del_idx + 1);
	}

	tcp::resolver resolver(io_context);
	auto          endpoint_it = resolver.resolve(tcp::resolver::query(hostname, port));

	socket.connect(*endpoint_it);
}

client_session::~client_session() {
	stop = true;
	io_context.stop();
	io_thread.join();
}

void
client_session::start_thread() {
	stop      = false;
	io_thread = std::thread(&client_session::thread, this);
}

void
client_session::send_packet(
	packet_t&& packet) {
	auto data_for_send = packet->get_data_for_send();
	async_write(socket, buffer(data_for_send.data(), data_for_send.size()), transfer_all());
}

std::vector<packet_t>
client_session::get_received_packets() {
	std::lock_guard lock(mutex);
	return std::move(received_packets);
}
} // network
