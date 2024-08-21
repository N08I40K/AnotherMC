//
// Created by n08i40k on 13.08.2024.
//

#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H
#include <unordered_map>

#include "chunk.h"
#include "math/chunk_pos.h"
#include "world/chunk.h"

class chunk_renderer {
	std::unordered_map<chunk_pos, std::vector<gl_vao> > chunks;

	void rebuild_vao(chunk_pos pos);

public:
	chunk_renderer();
	~chunk_renderer();

	void
	add_chunk(
		chunk* chunks);
	void
	remove_chunk(
		chunk_pos chunk_pos);
};

#endif //CHUNK_RENDERER_H
