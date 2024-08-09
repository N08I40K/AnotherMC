//
// Created by n08i40k on 09.08.2024.
//

#include "gl_ebo.h"

#include <cassert>

gl_ebo::gl_ebo(
	VecT&& indices)
	: indices(std::move(indices)) {
	assert(!this->indices.empty());
	assert((this->indices.size() % 3) == 0);
}

gl_ebo::gl_ebo(
	gl_ebo&& other) noexcept: indices(std::move(other.indices)) { std::swap(ebo, other.ebo); }

gl_ebo&
gl_ebo::operator=(
	gl_ebo other) {
	std::swap(*this, other);
	return *this;
}

gl_ebo::~gl_ebo() {
	if (ebo != 0)
		glDeleteBuffers(1, &ebo);
}

void
gl_ebo::configure() {
	if (ebo != 0)
		glDeleteBuffers(1, &ebo);

	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             static_cast<GLsizeiptr>(indices.size()) * static_cast<GLsizeiptr>(sizeof(GLfloat)),
	             indices.data(),
	             GL_STATIC_DRAW);
}

GLuint
gl_ebo::get_ebo() const { return ebo; }

gl_ebo::VecT
gl_ebo::get_indices() const { return indices; }
