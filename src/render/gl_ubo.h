//
// Created by n08i40k on 11.08.2024.
//

#ifndef GL_UBO_H
#define GL_UBO_H
#include <memory>
#include <GL/glew.h>

class gl_ubo {
	GLuint ubo{};

	GLuint size{};

public:
	explicit
	gl_ubo(
		GLuint size);

	gl_ubo(
		const gl_ubo& other) = delete;

	gl_ubo(
		gl_ubo&& other) noexcept;

	gl_ubo&
	operator=(
		gl_ubo other);

	~gl_ubo();

	void
	set_data(
		const void* data) const;

	[[nodiscard]] GLuint
	get_ubo() const;
};

using gl_ubo_ptr = std::unique_ptr<gl_ubo>;

#endif //GL_UBO_H
