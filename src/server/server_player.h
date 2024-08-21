//
// Created by n08i40k on 20.08.2024.
//

#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H
#include <memory>
#include <string>

namespace entities {
class player;
}

namespace network {
class server_session;
}

class server_player {
	std::shared_ptr<network::server_session> session;
	std::string                              nickname;

	std::weak_ptr<entities::player> entity;

	bool disconnected{};

public:
	explicit
	server_player(
		std::shared_ptr<network::server_session> session);

	void
	disconnect(
		const std::string& reason);

	[[nodiscard]] bool
	is_disconnected() const { return disconnected; }

	[[nodiscard]] std::weak_ptr<network::server_session>
	get_session() const { return session; }

	[[nodiscard]] const std::string&
	get_nickname() const { return nickname; }

	[[nodiscard]] const std::weak_ptr<entities::player>&
	get_entity() const { return entity; }
};

#endif //SERVER_PLAYER_H
