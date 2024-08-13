//
// Created by n08i40k on 12.08.2024.
//

#ifndef MODELS_FULL_BLOCK_H
#define MODELS_FULL_BLOCK_H
#include <utility>
#include <vector>
#include <GL/glew.h>
#include <glm/vec4.hpp>

namespace models {
struct full_block {
	enum class rotation_angle {
		NEG_Z,
		NEG_X,
		POS_Z,
		POS_X
	};

	const glm::vec4* front_uv;
	const glm::vec4* back_uv;
	const glm::vec4* top_uv;
	const glm::vec4* bottom_uv;
	const glm::vec4* right_uv;
	const glm::vec4* left_uv;

	explicit
	full_block(
		const glm::vec4* uv);

	full_block(
		const glm::vec4* front_uv,
		const glm::vec4* uv);

	full_block(
		const glm::vec4* front_uv,
		const glm::vec4* top_uv,
		const glm::vec4* bottom_uv,
		const glm::vec4* uv);

	void
	rotate(
		rotation_angle angle);

	[[nodiscard]] std::pair<std::vector<GLfloat>, std::vector<GLuint> >
	build() const;
};
} // models

#endif //MODELS_FULL_BLOCK_H
