//
// Created by n08i40k on 09.08.2024.
//

#ifndef GL_EBO_H
#define GL_EBO_H
#include <memory>
#include <vector>
#include <GL/glew.h>

class gl_ebo {
public:
	using VecT = std::vector<GLuint>;

private:
	GLuint ebo{};
	VecT   indices;

public:
	explicit
	gl_ebo(
		VecT  indices);

	gl_ebo(
		const gl_ebo& other) = delete;

	gl_ebo(
		gl_ebo&& other) noexcept;

	gl_ebo&
	operator=(
		gl_ebo other);

	~gl_ebo();

	void
	configure();

	void set_slot(const char* name, GLuint index);

	[[nodiscard]] GLuint
	get_ebo() const;

	[[nodiscard]] VecT
	get_indices() const;
};

using gl_ebo_ptr = std::unique_ptr<gl_ebo>;

#endif //GL_EBO_H
