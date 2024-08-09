//
// Created by n08i40k on 09.08.2024.
//

#include "gl_vao.h"

gl_vao::gl_vao(
	gl_vbo&&                vbo,
	std::optional<gl_ebo>&& ebo)
	: vbo(std::move(vbo))
	, ebo(std::move(ebo)) {}

gl_vao::gl_vao(
	gl_vao&& other) noexcept
	: vbo(std::move(other.vbo))
	, ebo(std::move(other.ebo)) { std::swap(vao, other.vao); }

gl_vao&
gl_vao::operator=(
	gl_vao other) {
	std::swap(*this, other);
	return *this;
}

gl_vao::~gl_vao() {
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);
}

void
gl_vao::configure() {
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	if (ebo.has_value())
		ebo->configure();
	vbo.configure();

	glBindVertexArray(0);
}

void
gl_vao::draw() const {
	glBindVertexArray(vao);

	if (ebo) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->get_ebo());
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ebo->get_indices().size()), GL_UNSIGNED_INT, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vbo.get_vertices().size()));

	glBindVertexArray(0);
}
