//
// Created by n08i40k on 09.08.2024.
//

#ifndef GL_VBO_H
#define GL_VBO_H
#include <vector>
#include <GL/glew.h>

class gl_vbo {
	std::vector<GLfloat> vertices;
	GLuint               vbo{};

public:
	explicit
	gl_vbo(
		std::vector<GLfloat> vertices);

	gl_vbo(
		const gl_vbo& other) = delete;

	gl_vbo(
		gl_vbo&& other) noexcept;

	gl_vbo&
	operator=(
		gl_vbo other);

	~gl_vbo();

	void
	configure();

	[[nodiscard]] const std::vector<GLfloat>&
	get_vertices() const;
};

#endif //GL_VBO_H
