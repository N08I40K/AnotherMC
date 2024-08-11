//
// Created by n08i40k on 09.08.2024.
//

#ifndef GL_VAO_H
#define GL_VAO_H
#include <memory>
#include <vector>
#include <GL/glew.h>

class gl_vao {
	GLuint vao{};

	GLuint vbo{};
	GLuint ebo{};

	GLsizei vbo_size{};
	GLsizei ebo_size{};

	void
	create_vbo(
		const GLfloat* vertices,
		size_t         count);
	void
	create_ebo(
		const GLuint* indices,
		size_t        count);

public:
	gl_vao(
		const GLfloat* vertices,
		size_t         vertices_count,
		const GLuint*  indices,
		size_t         indices_count);

	~gl_vao();

	[[nodiscard]] GLuint
	get_vao() const { return vao; }

	[[nodiscard]] GLuint
	get_vbo() const { return vbo; }

	[[nodiscard]] GLuint
	get_ebo() const { return ebo; }

	[[nodiscard]] GLsizei
	get_vbo_size() const { return vbo_size; }

	[[nodiscard]] GLsizei
	get_ebo_size() const { return ebo_size; }
};

using gl_vao_ptr = std::unique_ptr<gl_vao>;

#endif //GL_VAO_H
