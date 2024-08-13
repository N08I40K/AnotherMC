//
// Created by n08i40k on 13.08.2024.
//

#include "chunk.h"

#include "client/model/full_block.h"
#include "world/block/simple_block.h"



namespace client {
chunk::chunk(
	const chunk_pos pos)
	: chunk(pos)
	, vao(nullptr, 0, nullptr, 0) {
	for (int32_t x{}; x < chunk_pos::max_xz; ++x) {
		for (int32_t z{}; z < chunk_pos::max_xz; ++z) {
			get_data()[x][0][z] = std::make_unique<simple_block>("stone", block_pos({x, 0, z}));
		}
	}

	regenerate_vao();
}

chunk::~chunk() {}

void
chunk::regenerate_vao() {

}
}
