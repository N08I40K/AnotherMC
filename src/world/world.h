//
// Created by n08i40k on 13.08.2024.
//

#ifndef WORLD_H
#define WORLD_H
#include <unordered_map>

#include "block.h"
#include "math/chunk_pos.h"

class block_pos;
class chunk;

class world {
	std::unordered_map<chunk_pos, chunk> chunks;

protected:
	std::unordered_map<chunk_pos, chunk>&
	get_chunks() { return chunks; };

public:
	world();
	virtual ~world();

	block*
	get_block(
		block_pos pos);
};

#endif //WORLD_H
