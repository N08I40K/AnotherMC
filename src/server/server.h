//
// Created by n08i40k on 13.08.2024.
//

#ifndef SERVER_H
#define SERVER_H
#include <memory>

#include "world/world.h"

class server_player;

namespace network {
class server_session;
}

namespace network {
class server;
}

class server {
	std::unique_ptr<network::server> network;

	void
	on_new_connection(
		std::shared_ptr<network::server_session> session);

	std::vector<std::shared_ptr<server_player> > players;

public:
	server();
	~server();

	void
	tick();
};

#endif //SERVER_H
