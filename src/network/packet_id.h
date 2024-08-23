//
// Created by n08i40k on 20.08.2024.
//

#ifndef PACKET_ID_H
#define PACKET_ID_H
#include <cstdint>
#include <memory>

namespace network {
class transfer_context;

using packet_id_t = uint32_t;
using packet_t  = std::unique_ptr<transfer_context>;

enum struct packet_id : packet_id_t {
	CONNECT_REQUEST = 0,
	DISCONNECT,
	S2C_SPAWN
};
}

#endif //PACKET_ID_H
