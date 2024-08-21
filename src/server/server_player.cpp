//
// Created by n08i40k on 20.08.2024.
//

#include "server_player.h"
#include "s2c_disconnect.pb.h"
#include "network/packet_id.h"
#include "network/transfer_context.h"

server_player::server_player(
	std::shared_ptr<network::server_session> session): session(std::move(session)) {}

void
server_player::disconnect(
	const std::string& reason) {
	disconnected = true;

	s2c_disconnect packet;
	packet.set_reason(reason);

	network::transfer_context::create_send(network::packet_id::DISCONNECT, packet.SerializeAsString());
}
