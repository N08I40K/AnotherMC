//
// Created by n08i40k on 13.08.2024.
//

#include "server.h"
#include "network/server/server.h"

#include "server_player.h"
#include "network/transfer_context.h"
#include "network/server/server_session.h"

void
server::on_new_connection(
	std::shared_ptr<network::server_session> session) {
	players.emplace_back(std::make_shared<server_player>(std::move(session)));
}

server::server() {
	network = std::make_unique<::network::server>("127.0.0.1", 20512);
	network->set_new_connection_handler([this](
	auto&& session) {
			this->on_new_connection(session);
		});
	network->start_thread();
}

server::~server() = default;

void
server::tick() {
	for (auto& player : players) {
		auto session = player->get_session().lock();

		for (auto  packets = session->get_received_packets();
		     auto& packet : packets) {
			if (player->is_disconnected())
				break;

			switch (auto [packet_id, packet_data] = packet->take_data(); packet_id) {
			case network::packet_id::CONNECT_REQUEST: { break; }
			default: player->disconnect("Unknown packet id!");
			}
		}
	}
}
