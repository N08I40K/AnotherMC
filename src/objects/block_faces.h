//
// Created by n08i40k on 11.08.2024.
//

#ifndef BLOCK_FACES_H
#define BLOCK_FACES_H
#include <cstddef>
#include <vector>

namespace block_faces {
static constexpr std::array<size_t, 8> FACE_UV{
	0, 1,
	0, 3,
	2, 3,
	2, 1
};

static constexpr std::array ALL_VERTICES_POS{
	-.5F, -.5F, -.5F,
	-.5F, .5F, -.5F,
	.5F, .5F, -.5F,
	.5F, -.5F, -.5F,

	.5F, -.5F, .5F,
	.5F, .5F, .5F,
	-.5F, .5F, .5F,
	-.5F, -.5F, .5F,
};
#define VERTICE_PACK(idx) ALL_VERTICES_POS[idx * static_cast<size_t>(3)], ALL_VERTICES_POS[(idx * static_cast<size_t>(3)) + 1], ALL_VERTICES_POS[(idx * static_cast<size_t>(3)) + 2]
static constexpr std::array<GLfloat, static_cast<size_t>(6 * 24)> ALL_VERTICES{
	// front
	VERTICE_PACK(0), 0, 0,
	VERTICE_PACK(1), 0, 0,
	VERTICE_PACK(2), 0, 0,
	VERTICE_PACK(3), 0, 0,

	// back
	VERTICE_PACK(4), 0, 0,
	VERTICE_PACK(5), 0, 0,
	VERTICE_PACK(6), 0, 0,
	VERTICE_PACK(7), 0, 0,

	// top
	VERTICE_PACK(1), 0, 0,
	VERTICE_PACK(6), 0, 0,
	VERTICE_PACK(5), 0, 0,
	VERTICE_PACK(2), 0, 0,

	// bottom
	VERTICE_PACK(7), 0, 0,
	VERTICE_PACK(0), 0, 0,
	VERTICE_PACK(3), 0, 0,
	VERTICE_PACK(4), 0, 0,

	// right
	VERTICE_PACK(3), 0, 0,
	VERTICE_PACK(2), 0, 0,
	VERTICE_PACK(5), 0, 0,
	VERTICE_PACK(4), 0, 0,

	// left
	VERTICE_PACK(7), 0, 0,
	VERTICE_PACK(6), 0, 0,
	VERTICE_PACK(1), 0, 0,
	VERTICE_PACK(0), 0, 0,
};
#undef VERTICE_PACK

static constexpr std::array<GLuint, 36> ALL_INDICES = {
	// front
	0, 1, 2,
	2, 3, 0,

	// back
	4, 5, 6,
	6, 7, 4,

	// top
	8, 9, 10,
	10, 11, 8,

	// bottom
	12, 13, 14,
	14, 15, 12,

	// right
	16, 17, 18,
	18, 19, 16,

	// left
	20, 21, 22,
	22, 23, 20
};

template <size_t Size>
constexpr void
emplace_block_face(
	std::array<GLuint, Size>& block,
	size_t&                   written,
	const size_t              face_offset) {
	for (size_t i{}; i < 6; ++i)
		block[written + i] = ALL_INDICES[face_offset + i];
	written += 6;
}

constexpr void
emplace_block_face(
	std::vector<GLuint>& block,
	const size_t         face_offset) {
	block.resize(block.size() + 6);

	auto* start = &block[block.size() - 6];
	for (size_t i{}; i < 6; ++i)
		start[i] = ALL_INDICES[face_offset + i];
}

// ReSharper disable CppDFAConstantParameter
// ReSharper disable CppDFAConstantConditions

static std::vector<GLuint>
construct_block(
	const bool front,
	const bool back,
	const bool top,
	const bool bottom,
	const bool right,
	const bool left) {
	std::vector<GLuint> block;
	if (front)
		emplace_block_face(block, 0);

	if (back)
		emplace_block_face(block, 6);

	if (top)
		emplace_block_face(block, 12);

	if (bottom)
		emplace_block_face(block, 18);

	if (right)
		emplace_block_face(block, 24);

	if (left)
		emplace_block_face(block, 30);

	return block;
}

template <bool Front, bool Back, bool Top, bool Bottom, bool Right, bool Left>
constexpr std::array<GLuint, static_cast<const size_t>(Front + Back + Top + Bottom + Right + Left) * 6>
construct_block_static() {
	constexpr auto         array_size = static_cast<const size_t>((Front + Back + Top + Bottom + Right + Left) * 6);
	std::array<GLuint, 36> block{};

	size_t written = 0;
	if (Front)
		emplace_block_face(block, written, 0);

	if (Back)
		emplace_block_face(block, written, 6);

	if (Top)
		emplace_block_face(block, written, 12);

	if (Bottom)
		emplace_block_face(block, written, 18);

	if (Bottom)
		emplace_block_face(block, written, 24);

	if (Left)
		emplace_block_face(block, written, 30);

	std::array<GLuint, array_size> stripped_block{};

	for (size_t i{}; i < array_size; ++i)
		stripped_block[i] = block[i];

	return stripped_block;
}

// ReSharper restore CppDFAConstantConditions
// ReSharper restore CppDFAConstantParameter

static constexpr auto FULL_BLOCK = construct_block_static<true, true, true, true, true, true>();

static void
face_shade_uv(
	GLfloat*&        vertices,
	const glm::vec4& uv) {
#define BUF_OFFSET_X(idx) ((5 * (idx)) + 3)
#define BUF_OFFSET_Y(idx) ((5 * (idx)) + 4)
	// (-, -)
	// (-, +)
	// (+, -)
	// (+, +)

	vertices[BUF_OFFSET_X(0)] = uv[FACE_UV[0]];
	vertices[BUF_OFFSET_X(1)] = uv[FACE_UV[2]];
	vertices[BUF_OFFSET_X(2)] = uv[FACE_UV[4]];
	vertices[BUF_OFFSET_X(3)] = uv[FACE_UV[6]];

	vertices[BUF_OFFSET_Y(0)] = uv[FACE_UV[1]];
	vertices[BUF_OFFSET_Y(1)] = uv[FACE_UV[3]];
	vertices[BUF_OFFSET_Y(2)] = uv[FACE_UV[5]];
	vertices[BUF_OFFSET_Y(3)] = uv[FACE_UV[7]];

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

inline void
block_shade_exclude_rl(
	GLfloat*         vertices,
	const glm::vec4& front_uv,
	const glm::vec4& back_uv,
	const glm::vec4& top_uv,
	const glm::vec4& bottom_uv,
	const glm::vec4& side_uv
) { block_shade_uv(vertices, &front_uv, &back_uv, &top_uv, &bottom_uv, &side_uv, &side_uv); }

inline void
block_shade_uv_exclude_side(
	GLfloat*         vertices,
	const glm::vec4& top_uv,
	const glm::vec4& bottom_uv,
	const glm::vec4& side_uv
) { block_shade_uv(vertices, &side_uv, &side_uv, &top_uv, &bottom_uv, &side_uv, &side_uv); }

inline void
block_shade_uv_all(
	GLfloat*         vertices,
	const glm::vec4& uv
) { block_shade_uv(vertices, &uv, &uv, &uv, &uv, &uv, &uv); }
}

#endif //BLOCK_FACES_H
