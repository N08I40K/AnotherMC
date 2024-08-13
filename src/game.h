//
// Created by n08i40k on 13.08.2024.
//

#ifndef GAME_H
#define GAME_H
#include <memory>

#include "registry/registries.h"

class client;
using client_ptr = std::unique_ptr<client>;

class server;
using server_ptr = std::unique_ptr<server>;

class game {
	static std::unique_ptr<game> instance;

	registries registries;

	client_ptr client{nullptr};
	server_ptr server{nullptr};

	bool server_tick{false};

	game();

public:
	~game();

	static void
	init();
	static game&
	get_instance();

	void
	tick();

	[[nodiscard]] const ::registries&
	get_registries() const { return registries; }

	[[nodiscard]] const client_ptr&
	get_client() const { return client; }

	[[nodiscard]] const server_ptr&
	get_server() const { return server; }

	[[nodiscard]] bool
	is_server_tick() const { return server_tick; }

	[[nodiscard]] bool
	is_client_tick() const { return !server_tick; }
};

#endif //GAME_H
