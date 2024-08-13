//
// Created by n08i40k on 13.08.2024.
//

#include "world.h"

#include "chunk.h"

world::world() { load_chunk({0, 0}); }

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

void
world::load_chunk(
	chunk_pos pos) {
	if (chunks.contains(pos))
		return;

	chunks.emplace(pos, chunk{pos});
}

void
world::unload_chunk(
	const chunk_pos pos) {
	if (!chunks.contains(pos))
		return;

	chunks.erase(pos);
}
