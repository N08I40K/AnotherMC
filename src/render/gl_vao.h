//
// Created by n08i40k on 09.08.2024.
//

#ifndef GL_VAO_H
#define GL_VAO_H
#include <memory>
#include <GL/glew.h>

#include "gl_ebo.h"
#include "gl_vbo.h"

class gl_vao {
	GLuint vao{};

	gl_vbo vbo;
	gl_ebo ebo;

public:
	explicit
	gl_vao(
		gl_vbo vbo,
		gl_ebo ebo);

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

	[[nodiscard]] GLuint
	get_vao() const { return vao; }

	[[nodiscard]] const gl_vbo&
	get_vbo() const { return vbo; }

	[[nodiscard]] const gl_ebo&
	get_ebo() const { return ebo; }
};

using gl_vao_ptr = std::unique_ptr<gl_vao>;

#endif //GL_VAO_H
