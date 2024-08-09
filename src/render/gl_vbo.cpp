//
// Created by n08i40k on 09.08.2024.
//

#include "gl_vbo.h"

#include <cassert>

gl_vbo::gl_vbo(
	std::vector<GLfloat>&& vertices)
	: vertices(std::move(vertices)) {
	assert(!this->vertices.empty());
	assert((this->vertices.size() % 3) == 0);
}

gl_vbo::gl_vbo(
	gl_vbo&& other) noexcept: vertices(std::move(other.vertices)) { std::swap(vbo, other.vbo); }

gl_vbo&
gl_vbo::operator=(
	gl_vbo other) {
	std::swap(*this, other);
	return *this;
}

gl_vbo::~gl_vbo() {
	if (vbo != 0)
		glDeleteBuffers(1, &vbo);
}

void
gl_vbo::configure() {
	if (vbo != 0)
		glDeleteBuffers(1, &vbo);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
	             static_cast<GLsizeiptr>(vertices.size()) * static_cast<GLsizeiptr>(sizeof(GLfloat)),
	             vertices.data(),
	             GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
}

const std::vector<GLfloat>&
gl_vbo::get_vertices() const { return vertices; }
