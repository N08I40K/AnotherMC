//
// Created by n08i40k on 09.08.2024.
//

#include "gl_vao.h"

void
gl_vao::create_vbo(
	const GLfloat* vertices,
	const size_t   count) {
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
	             static_cast<GLsizeiptr>(count) * static_cast<GLsizeiptr>(sizeof(GLfloat)),
	             vertices,
	             GL_STATIC_DRAW);

	constexpr GLuint stride = 5 * sizeof(GLfloat);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	vbo_size = static_cast<GLsizei>(count / stride);
}

void
gl_vao::create_ebo(
	const GLuint* indices,
	const size_t  count) {
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             static_cast<GLsizeiptr>(count) * static_cast<GLsizeiptr>(sizeof(GLfloat)),
	             indices,
	             GL_STATIC_DRAW);

	ebo_size = static_cast<GLsizei>(count);
}

gl_vao::gl_vao(
	const GLfloat* vertices,
	const size_t   vertices_count,
	const GLuint*  indices,
	const size_t   indices_count) {
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	create_ebo(indices, indices_count);
	create_vbo(vertices, vertices_count);

	glBindVertexArray(0);
}

gl_vao::~gl_vao() {
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);

	if (ebo != 0)
		glDeleteBuffers(1, &ebo);

	if (vbo != 0)
		glDeleteBuffers(1, &vbo);
}
