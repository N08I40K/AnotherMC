//
// Created by n08i40k on 12.08.2024.
//

#ifndef BLOCK_POS_H
#define BLOCK_POS_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "chunk_pos.h"

class block_pos {
	glm::ivec3 pos;

public:
	explicit
	block_pos(
		const glm::ivec3 pos) : pos(pos) {}

	block_pos(
		const int x,
		const int y,
		const int z) : pos(x, y, z) {}

	[[nodiscard]] glm::ivec3
	get_world_pos() const { return pos; }

	[[nodiscard]] chunk_pos
	get_chunk_pos() const {
		return chunk_pos{
			static_cast<int>(pos.x / chunk_pos::max_xz),
			static_cast<int>(pos.z / chunk_pos::max_xz)
		};
	}

	[[nodiscard]] glm::ivec3
	get_in_chunk_pos() const {
		return {
			static_cast<int>(pos.x % chunk_pos::max_xz),
			pos.y,
			static_cast<int>(pos.z % chunk_pos::max_xz)
		};
	}
};

#endif //BLOCK_POS_H
