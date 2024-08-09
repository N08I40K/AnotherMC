//
// Created by n08i40k on 09.08.2024.
//

#ifndef GL_EBO_H
#define GL_EBO_H
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
		VecT&& indices);

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

	[[nodiscard]] GLuint
	get_ebo() const;

	[[nodiscard]] VecT
	get_indices() const;
};

#endif //GL_EBO_H
