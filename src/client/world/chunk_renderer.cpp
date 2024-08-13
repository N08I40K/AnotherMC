//
// Created by n08i40k on 13.08.2024.
//

#include "chunk_renderer.h"

namespace {
__forceinline void
insert_face(
	std::vector<GLfloat>& vertex_buffer,
	GLfloat*              vertices,
	std::vector<GLuint>&  indices_buffer,
	GLuint*               indices) {
	vertex_buffer.insert(vertex_buffer.end(), vertices, vertices + 20);
	indices_buffer.insert(indices_buffer.end(), indices, indices + 6);
}

std::pair<std::vector<GLfloat>, std::vector<GLuint> >
exclude_block_faces(
	GLfloat*   vertices,
	GLuint*    indices,
	const bool front,
	const bool back,
	const bool top,
	const bool bottom,
	const bool right,
	const bool left) {
	const size_t faces_count =
			static_cast<size_t>(front) +
			static_cast<size_t>(back) +
			static_cast<size_t>(top) +
			static_cast<size_t>(bottom) +
			static_cast<size_t>(right) +
			static_cast<size_t>(left);
	const size_t result_vertices_count = 20LLU * faces_count;
	const size_t result_indices_count  = 6LLU * faces_count;

	std::vector<GLfloat> result_vertices;
	result_vertices.reserve(result_vertices_count);

	std::vector<GLuint> result_indices;
	result_indices.reserve(result_indices_count);

	if (front)
		insert_face(result_vertices, vertices, result_indices, indices);
	if (back)
		insert_face(result_vertices, vertices + 20, result_indices, indices + 6);
	if (top)
		insert_face(result_vertices, vertices + 40, result_indices, indices + 12);
	if (bottom)
		insert_face(result_vertices, vertices + 60, result_indices, indices + 18);
	if (right)
		insert_face(result_vertices, vertices + 80, result_indices, indices + 24);
	if (left)
		insert_face(result_vertices, vertices + 100, result_indices, indices + 30);

	return {std::move(result_vertices), std::move(result_indices)};
}
}

void
chunk_renderer::rebuild_vao(
	chunk_pos pos) {
		boost::multi_array<std::optional<models::full_block>, 3> models{
		boost::extents[chunk_pos::max_xz][chunk_pos::max_y][chunk_pos::max_xz]
	};

	auto& data = get_data();

	for (int64_t x{}; x < chunk_pos::max_xz; ++x) {
		for (int64_t y{}; y < chunk_pos::max_y; ++y) {
			for (int64_t z{}; z < chunk_pos::max_xz; ++z) {
				auto& block = data[x][y][z];

				if (block == nullptr || block->get_render_mode() == block::render_mode::OWN) {
					models[x][y][z] = std::nullopt;
					continue;
				}

				models[x][y][z] = block->get_model();
			}
		}
	}

	for (int64_t x{}; x < chunk_pos::max_xz; ++x) {
		for (int64_t y{}; y < chunk_pos::max_y; ++y) {
			for (int64_t z{}; z < chunk_pos::max_xz; ++z) {
				auto& opt_model = models[x][y][z];
				if (opt_model == std::nullopt)
					continue;

				auto& model = *opt_model;

				if (model.front_uv != nullptr && z != 0 && data[x][y][z - 1]->get_render_mode() !=
					block::render_mode::OWN) {
					model.front_uv               = nullptr;
					models[x][y][z - 1]->back_uv = nullptr;
				}

				if (model.back_uv != nullptr && z != (chunk_pos::max_xz - 1) && models[x][y][z + 1] != std::nullopt) {
					model.back_uv                 = nullptr;
					models[x][y][z + 1]->front_uv = nullptr;
				}

				if (model.top_uv != nullptr && y != (chunk_pos::max_y - 1) && data[x][y + 1][z]->get_render_mode() !=
					block::render_mode::OWN) {
					model.top_uv                   = nullptr;
					models[x][y + 1][z]->bottom_uv = nullptr;
				}

				if (model.bottom_uv != nullptr && y != 0 && data[x][y - 1][z]->get_render_mode() !=
					block::render_mode::OWN) {
					model.bottom_uv             = nullptr;
					models[x][y - 1][z]->top_uv = nullptr;
				}

				if (model.right_uv != nullptr && x != 0 && models[x - 1][y][z] != std::nullopt) {
					model.right_uv                   = nullptr;
					models[x - 1][y][z + 1]->left_uv = nullptr;
				}

				if (model.left_uv != nullptr && x != (chunk_pos::max_xz - 1) && models[x + 1][y][z] != std::nullopt) {
					model.left_uv                     = nullptr;
					models[x + 1][y][z + 1]->right_uv = nullptr;
				}
			}
		}
	}

	std::vector<GLfloat> vertices;
	std::vector<GLuint>  indices;

	for (int64_t x{}; x < chunk_pos::max_xz; ++x) {
		for (int64_t y{}; y < chunk_pos::max_y; ++y) {
			for (int64_t z{}; z < chunk_pos::max_xz; ++z) {
				auto& opt_model = models[x][y][z];
				if (opt_model == std::nullopt)
					continue;

				auto [model_vertices, model_indices] = opt_model->build();

				vertices.append_range(std::move(model_vertices));
				indices.append_range(std::move(model_indices));
			}
		}
	}

	vao = gl_vao(vertices.data(), vertices.size(), indices.data(), indices.size());
}

chunk_renderer::chunk_renderer() {}

chunk_renderer::~chunk_renderer() {}

void
chunk_renderer::add_chunk(
	chunk* chunks) {

}

void
chunk_renderer::remove_chunk(
	chunk_pos chunk_pos) {}
