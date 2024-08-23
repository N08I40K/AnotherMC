//
// Created by n08i40k on 13.08.2024.
//

#include "world.h"

#include "chunk.h"
#include "game.h"

#include <stduuid/uuid.h>

#include "entity/entity.h"

world::world() = default;

world::~world() = default;

block*
world::get_block(
	const block_pos pos) {
	const auto chunk_pos = pos.get_chunk_pos();

	auto iter = chunks.find(chunk_pos);
	if (iter == chunks.end())
		return nullptr;

	return iter->second[pos].get();
}

entity*
world::get_entity(
	const uuids::uuid uuid) {
	if (auto iter = entities.find(uuid);
		iter != entities.end())
		return iter->second.get();

	return nullptr;
}

uuids::uuid
world::add_entity(
	entity*          entity,
	const glm::vec3& position) {
	entity->set_position(position);

	const auto uuid = uuids::uuid_system_generator{}();
	entities.emplace(uuid, entity);

	return uuid;
}

chunk&
world::load_chunk(
	const chunk_pos pos) {
	if (const auto iter = chunks.find(pos);
		iter != chunks.end())
		return iter->second;

	chunks.emplace(pos, chunk{pos});
	auto& chunk = chunks.at(pos);

	auto* stone_instance = game::get_instance()
	                       .get_registries()
	                       .get_blocks()["stone"];

	for (int x{}; x < chunk_pos::max_xz; ++x) {
		for (int z{}; z < chunk_pos::max_xz; ++z)
			chunk[block_pos{x, 0, z}].reset(stone_instance->clone());
	}

	return chunk;
}

void
world::unload_chunk(
	const chunk_pos pos) {
	const auto iter = chunks.find(pos);
	if (iter == chunks.end())
		return;

	chunks.erase(iter);
}
