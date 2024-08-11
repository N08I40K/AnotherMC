//
// Created by n08i40k on 11.08.2024.
//

#include <GL/glew.h>
#include "gl_window.h"

#include <print>

bool
gl_window::init_glfw(
	const GLFWkeyfun key_callback) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Создание окна GLFW и его настройка
	window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
	if (window == nullptr) {
		std::println("Не удалось создать окно GLFW!");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	return true;
}

gl_window::gl_window(
	const GLFWkeyfun key_callback) {
	if (!init_glfw(key_callback))
		return;

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::println("Не удалось инициализировать GLEW!");
		glfwTerminate();
		return;
	}

	int width;
	int height;

	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);

	init_completed = true;
}

GLFWwindow*
gl_window::get_window() const { return window; }

bool
gl_window::is_init_completed() const { return init_completed; }
