#include <array>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <print>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "render/gl_vao.h"

namespace {

class gl_window {
	GLFWwindow* window{nullptr};
	bool        init_completed{false};

	GLuint shader_program{};

	std::vector<gl_vao> vaos;

	static void
	key_callback(
		GLFWwindow*                window,
		const int                  key,
		[[maybe_unused]] const int scancode,
		const int                  action,
		[[maybe_unused]] const int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	bool
	init_glfw() {
		// Инициализация GLFW
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

	static std::optional<std::vector<char> >
	read_shader_file(
		const std::filesystem::path& file_path) {
		if (!exists(file_path)) {
			std::println("Не удалось найти шейдер! {}", file_path.string());
			return std::nullopt;
		}

		if (!is_regular_file(file_path)) {
			std::println("Путь ведущий к шейдеру указывает не на файл! {}", file_path.string());
			return std::nullopt;
		}

		std::ifstream read_stream(file_path, std::ios::binary | std::ios::ate);
		const auto    size = read_stream.tellg();
		read_stream.seekg(0, std::ios::beg);

		std::vector<GLchar> contents(size);
		if (!read_stream.read(contents.data(), size)) {
			std::println("Не удалось прочитать файл шейдера! {}", file_path.string());
			return std::nullopt;
		}

		return std::move(contents);
	}

	// ReSharper disable CppDFAConstantParameter
	static bool
	load_shader(
		GLuint&                      shader,
		const GLsizei                count,
		const GLenum                 type,
		const std::filesystem::path& path) {
		shader                     = glCreateShader(type);
		const auto shader_contents = read_shader_file(path);
		if (!shader_contents.has_value())
			return false;

		const auto* shader_data = shader_contents->data();
		const auto  shader_size = static_cast<int>(shader_contents->size());

		glShaderSource(shader, count, &shader_data, &shader_size);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == 0) {
			std::array<GLchar, 512> info_log{};
			glGetShaderInfoLog(shader, info_log.size(), nullptr, info_log.data());

			std::println("Не удалось скомпилировать шейдер! {}\n{}", path.string(), info_log.data());
			return false;
		}

		return true;
	}

	// ReSharper restore CppDFAConstantParameter

	bool
	load_shaders() {
		const auto shaders_dir = std::filesystem::path{ASSERT_DIR} / "shaders";

		GLuint vertex_shader{};
		GLuint fragment_shader{};

		if (!(load_shader(vertex_shader, 1, GL_VERTEX_SHADER, shaders_dir / "vertex.glsl")
			&& load_shader(fragment_shader, 1, GL_FRAGMENT_SHADER, shaders_dir / "fragment.glsl")))
			return false;

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		GLint success;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			std::array<GLchar, 512> info_log{};
			glGetProgramInfoLog(shader_program, info_log.size(), nullptr, info_log.data());

			std::println("Не удалось связать шейдерную программу! {}", info_log.data());
			return false;
		}

		glUseProgram(shader_program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return true;
	}

public:
	gl_window() {
		if (!init_glfw())
			return;

		// Инициализация GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::println("Не удалось инициализировать GLEW!");
			glfwTerminate();
			return;
		}

		// Настройка окна Viewport
		int width;
		int height;

		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		if (!load_shaders())
			return;

		std::vector vertices{
			-0.5F, 0.5F, 0.0F,
			0.5F, 0.5F, 0.0F,
			-0.5F, -0.5F, 0.0F,
			0.5F, -0.5F, 0.0F,
		};
		std::vector<GLuint> indices{
			0, 1, 2,
			3, 1, 2,
		};
		vaos.emplace_back(gl_vbo{std::move(vertices)}, gl_ebo{std::move(indices)});

		for (auto& vao : vaos)
			vao.configure();

		init_completed = true;
	}

	void
	draw() const {
		glUseProgram(shader_program);

		for (const auto& vao : vaos)
			vao.draw();
	}

	void
	infinity_poll() const {
		// Цикл отрисовки

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		while (glfwWindowShouldClose(window) == 0) {
			glfwPollEvents();

			glClearColor(0.F, 0.F, 1.F, 1.F);
			glClear(GL_COLOR_BUFFER_BIT);

			draw();

			glfwSwapBuffers(window);
		}

		glfwTerminate();
	}

	[[nodiscard]] GLFWwindow*
	get_window() const { return window; }

	[[nodiscard]] bool
	is_init_completed() const { return init_completed; }
};
}

int
main() {
	gl_window window{};

	if (!window.is_init_completed())
		return -1;

	window.infinity_poll();

	return 0;
}
