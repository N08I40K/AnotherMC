//
// Created by n08i40k on 13.08.2024.
//

#include "game.h"

#include "client/client.h"
#include "server/server.h"

std::unique_ptr<game> game::instance = nullptr;

game::game() {
	server = std::make_unique<::server>();
	client = std::make_unique<::client>();
}

void
game::init() { instance = std::unique_ptr<game>(new game()); }

game&
game::get_instance() { return *instance; }

void
game::tick() {
	if (server != nullptr) {
		server_tick = true;
		server->tick();
	}

	if (client != nullptr) {
		server_tick = false;
		client->tick();
	}
}

game::~game() = default;
