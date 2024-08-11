//
// Created by n08i40k on 11.08.2024.
//

#include "gl_ubo.h"

gl_ubo::gl_ubo(
	const GLuint size): size{size} {
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

gl_ubo::gl_ubo(
	gl_ubo&& other) noexcept: ubo(other.ubo)
	                        , size(other.size) { other.ubo = 0; }

gl_ubo&
gl_ubo::operator=(
	gl_ubo other) {
	std::swap(*this, other);
	return *this;
}

gl_ubo::~gl_ubo() {
	if (ubo != 0)
		glDeleteBuffers(1, &ubo);
}

GLuint
gl_ubo::get_ubo() const { return ubo; }

void
gl_ubo::set_data(
	const void* data) const {
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
