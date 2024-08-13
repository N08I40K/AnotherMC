//
// Created by n08i40k on 13.08.2024.
//

#ifndef SERVER_WORLD_H
#define SERVER_WORLD_H
#include "game.h"
#include "world/world.h"

class server_world : public world {
public:
	void
	load_chunk(
		chunk_pos pos) {
		auto& chunks = get_chunks();

		if (chunks.contains(pos))
			return;

		auto& block_registry = game::
	}
};

#endif //SERVER_WORLD_H
