//
// Created by n08i40k on 13.08.2024.
//

#ifndef CLIENT_CHUNK_H
#define CLIENT_CHUNK_H
#include "client/render/gl_vao.h"
#include "world/chunk.h"

namespace client {
class chunk : public ::chunk {
	gl_vao vao;

public:
	explicit
	chunk(
		const chunk_pos pos);
	~chunk();

	void
	regenerate_vao();

	[[nodiscard]] const gl_vao&
	get_vao() const { return vao; }
};
}

#endif //CLIENT_CHUNK_H
