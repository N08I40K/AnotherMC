//
// Created by n08i40k on 12.08.2024.
//

#include "full_block.h"

#include <array>
#include <cstddef>
#include <stdexcept>
#include <glm/vec4.hpp>

#include "client/objects/block_faces.h"

namespace models {
full_block::full_block(
	const glm::vec4* uv)
	: front_uv(uv)
	, back_uv(uv)
	, top_uv(uv)
	, bottom_uv(uv)
	, right_uv(uv)
	, left_uv(uv) {}

full_block::full_block(
	const glm::vec4* front_uv,
	const glm::vec4* uv)
	: front_uv(front_uv)
	, back_uv(uv)
	, top_uv(uv)
	, bottom_uv(uv)
	, right_uv(uv)
	, left_uv(uv) {}

full_block::full_block(
	const glm::vec4* front_uv,
	const glm::vec4* top_uv,
	const glm::vec4* bottom_uv,
	const glm::vec4* uv)
	: front_uv(front_uv)
	, back_uv(uv)
	, top_uv(top_uv)
	, bottom_uv(bottom_uv)
	, right_uv(uv)
	, left_uv(uv) {}

void
full_block::rotate(
	const rotation_angle angle) {
	if (angle == rotation_angle::NEG_Z)
		return;

	const auto* front = front_uv;
	const auto* right = right_uv;
	const auto* back  = back_uv;
	const auto* left  = left_uv;

	switch (angle) {
	case rotation_angle::NEG_X: {
		front_uv = left;
		right_uv = front;
		back_uv  = right;
		left_uv  = back;
		break;
	}
	case rotation_angle::POS_Z: {
		front_uv = back;
		right_uv = left;
		back_uv  = front;
		left_uv  = right;
		break;
	}
	case rotation_angle::POS_X: {
		front_uv = right;
		right_uv = back;
		back_uv  = left;
		left_uv  = front;
	}
	default: throw std::runtime_error("Передан неизвестный угол поворота!");
	}
}

static void
face_shade_uv(
	GLfloat*&        vertices,
	const glm::vec4& uv) {
#define BUF_OFFSET_X(idx) ((5 * (idx)) + 3)
#define BUF_OFFSET_Y(idx) ((5 * (idx)) + 4)

	vertices[BUF_OFFSET_X(0)] = uv[block_faces::FACE_UV[0]];
	vertices[BUF_OFFSET_X(1)] = uv[block_faces::FACE_UV[2]];
	vertices[BUF_OFFSET_X(2)] = uv[block_faces::FACE_UV[4]];
	vertices[BUF_OFFSET_X(3)] = uv[block_faces::FACE_UV[6]];

	vertices[BUF_OFFSET_Y(0)] = uv[block_faces::FACE_UV[1]];
	vertices[BUF_OFFSET_Y(1)] = uv[block_faces::FACE_UV[3]];
	vertices[BUF_OFFSET_Y(2)] = uv[block_faces::FACE_UV[5]];
	vertices[BUF_OFFSET_Y(3)] = uv[block_faces::FACE_UV[7]];

#undef BUF_OFFSET_X
#undef BUF_OFFSET_Y

	vertices += 20;
}

static void
block_shade_uv(
	GLfloat*         vertices,
	const glm::vec4* front_uv,
	const glm::vec4* back_uv,
	const glm::vec4* top_uv,
	const glm::vec4* bottom_uv,
	const glm::vec4* right_uv,
	const glm::vec4* left_uv
) {
	if (front_uv != nullptr)
		face_shade_uv(vertices, *front_uv);

	if (back_uv != nullptr)
		face_shade_uv(vertices, *back_uv);

	if (top_uv != nullptr)
		face_shade_uv(vertices, *top_uv);

	if (bottom_uv != nullptr)
		face_shade_uv(vertices, *bottom_uv);

	if (right_uv != nullptr)
		face_shade_uv(vertices, *right_uv);

	if (left_uv != nullptr)
		face_shade_uv(vertices, *left_uv);
}

void
write_face(
	GLfloat*&     vertices,
	GLuint*&      indices,
	const uint8_t face_idx) {
	memcpy(vertices, block_faces::ALL_VERTICES.data() + static_cast<uintptr_t>(face_idx * 20), 20 * sizeof(GLuint));
	memcpy(indices, block_faces::ALL_INDICES.data() + static_cast<uintptr_t>(face_idx * 6), 6 * sizeof(GLuint));
	vertices += 20;
	indices += 6;
}

std::pair<std::vector<GLfloat>, std::vector<GLuint> >
full_block::build() const {
#define CHECK_UV(uv) static_cast<size_t>((uv) != nullptr)
	const size_t faces =
			CHECK_UV(front_uv) +
			CHECK_UV(back_uv) +
			CHECK_UV(top_uv) +
			CHECK_UV(bottom_uv) +
			CHECK_UV(right_uv) +
			CHECK_UV(left_uv);
#undef CHECK_UV

	const size_t vertices_count = faces * 4 * 5;
	const size_t indices_count  = faces * 6;

	auto* vertices = static_cast<GLfloat*>(alloca(vertices_count * sizeof(GLfloat)));
	auto* indices  = static_cast<GLuint*>(alloca(indices_count * sizeof(GLuint)));

	auto* vertices_write = vertices;
	auto* indices_write  = indices;

	if (front_uv != nullptr)
		write_face(vertices_write, indices_write, 0);

	if (back_uv != nullptr)
		write_face(vertices_write, indices_write, 1);

	if (top_uv != nullptr)
		write_face(vertices_write, indices_write, 2);

	if (bottom_uv != nullptr)
		write_face(vertices_write, indices_write, 3);

	if (right_uv != nullptr)
		write_face(vertices_write, indices_write, 4);

	if (left_uv != nullptr)
		write_face(vertices_write, indices_write, 5);

	vertices_write = vertices;
	block_shade_uv(vertices_write, front_uv, back_uv, top_uv, bottom_uv, right_uv, left_uv);

	return std::make_pair(
		std::vector<GLfloat>{vertices, vertices + vertices_count},
		std::vector<GLuint>{indices, indices + indices_count});
}
} // models
