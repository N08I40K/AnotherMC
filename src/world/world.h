//
// Created by n08i40k on 13.08.2024.
//

#ifndef WORLD_H
#define WORLD_H
#include <unordered_map>

#include "block.h"
#include "math/chunk_pos.h"

class entity;
class block_pos;
class chunk;

using entity_ptr = std::unique_ptr<entity>;

namespace uuids {
class uuid;
}

class world {
	std::unordered_map<chunk_pos, chunk>                      chunks;
	std::unordered_map<uuids::uuid, std::unique_ptr<entity> > entities;

protected:
	std::unordered_map<chunk_pos, chunk>&
	get_chunks() { return chunks; };

public:
	world();
	~world();

	block*
	get_block(
		block_pos pos);

	[[nodiscard]] std::unordered_map<uuids::uuid, std::unique_ptr<entity> >
	get_entities() const { return entities; }

	[[nodiscard]] entity*
	get_entity(
		uuids::uuid uuid);

	uuids::uuid
	add_entity(
		entity*          entity,
		const glm::vec3& position);

	chunk&
	load_chunk(
		chunk_pos pos);

	void
	unload_chunk(
		chunk_pos pos);
};

#endif //WORLD_H
