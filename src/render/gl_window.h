//
// Created by n08i40k on 11.08.2024.
//

#ifndef GL_WINDOW_H
#define GL_WINDOW_H
#include <GLFW/glfw3.h>

class gl_window {
	GLFWwindow* window{nullptr};
	bool        init_completed{false};

	bool
	init_glfw(
		GLFWkeyfun key_callback);

public:
	explicit
	gl_window(
		GLFWkeyfun key_callback);

	[[nodiscard]] GLFWwindow*
	get_window() const;

	[[nodiscard]] bool
	is_init_completed() const;
};

#endif //GL_WINDOW_H
