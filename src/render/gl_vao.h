//
// Created by n08i40k on 09.08.2024.
//

#ifndef GL_VAO_H
#define GL_VAO_H
#include <optional>
#include <GL/glew.h>

#include "gl_ebo.h"
#include "gl_vbo.h"

class gl_vao {
	GLuint vao{};

	gl_vbo                vbo;
	std::optional<gl_ebo> ebo;

public:
	explicit
	gl_vao(
		gl_vbo&&                vbo,
		std::optional<gl_ebo>&& ebo);

	gl_vao(
		const gl_vao& other) = delete;

	gl_vao(
		gl_vao&& other) noexcept;

	gl_vao&
	operator=(
		gl_vao other);

	~gl_vao();

	void
	configure();

	void
	draw() const;
};

#endif //GL_VAO_H
