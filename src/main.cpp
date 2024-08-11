#include <array>
#include <thread>
#include <chrono>
#include <filesystem>
#include <print>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assets/textures.h"
#include "render/gl_vao.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/gl_shader_program.h"
#include "render/gl_ubo.h"

namespace {
struct gl_vertex_uniform_data {
	glm::mat4 transform;
};

class gl_render_engine {
public:
	gl_render_engine()  = delete;
	~gl_render_engine() = delete;

	static void
	swap(
		GLFWwindow* window) { glfwSwapBuffers(window); }

	static void
	clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3F, 0.3F, 0.3F, 1.0f);
	}

	static void
	set_vao(
		const gl_vao_ptr& vao) {
		glBindVertexArray(vao->get_vao());
	}

	static void
	set_ubo(
		const gl_ubo_ptr& ubo,
		const GLuint      slot) {
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, ubo->get_ubo());
	}

	static void
	set_texture(
		const GLuint texture) { glBindTexture(GL_TEXTURE_2D, texture); }

	static void
	set_shader_program(
		const gl_shader_program_ptr& shader_program) {
		glUseProgram(shader_program->get_shader_program());
	}

	static void
	render(
		const GLsizei count) { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); }
};

class gl_window {
	GLFWwindow* window{nullptr};
	bool        init_completed{false};

	bool
	init_glfw(
		const GLFWkeyfun key_callback) {
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

public:
	explicit
	gl_window(
		const GLFWkeyfun key_callback) {
		if (!init_glfw(key_callback))
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

		init_completed = true;
	}

	[[nodiscard]] GLFWwindow*
	get_window() const { return window; }

	[[nodiscard]] bool
	is_init_completed() const { return init_completed; }
};

class game {
public:
	using clock = std::chrono::high_resolution_clock;
	using time_point = clock::time_point;

private:
	gl_window window;

	gl_shader_program_ptr shader_program;
	gl_vao_ptr            vao;
	gl_ubo_ptr            ubo;
	assets::textures_ptr  textures;

	bool init_completed{false};

	time_point frame_time_point{clock::now()};
	float delta_time{0.F};

	[[nodiscard]] gl_vao_ptr
	generate_sample() const {
		const auto uv = textures->get_atlas()->get_uvs().at("yellow_wool");


#define UV_MIN_X(uv) uv.x
#define UV_MIN_Y(uv) uv.y
#define UV_MAX_X(uv) uv.z
#define UV_MAX_Y(uv) uv.w
		std::vector vertices{
			// Position				// Color				// UV
			0.5F, 0.5F, 0.0F,		1.F, 1.F, 1.F, 1.F,		UV_MAX_X(uv), UV_MAX_Y(uv),
			0.5F, -0.5F, 0.0F,		1.F, 1.F, 1.F, 1.F,		UV_MAX_X(uv), UV_MIN_Y(uv),
			-0.5F, -0.5F, 0.0F,		1.F, 1.F, 1.F, 1.F,		UV_MIN_X(uv), UV_MIN_Y(uv),
			-0.5F, 0.5F, 0.0F,		1.F, 1.F, 1.F, 1.F,		UV_MIN_X(uv), UV_MAX_Y(uv),
		};
#undef UV_MIN_X
#undef UV_MIN_Y
#undef UV_MAX_X
#undef UV_MAX_Y
		std::vector<GLuint> indices{
			0, 1, 3,
			1, 2, 3,
		};

		gl_ubo ubo{sizeof(gl_vertex_uniform_data)};
		return std::make_unique<gl_vao>(gl_vbo{vertices}, gl_ebo{indices});
	}

public:
	game()
		: window(key_callback) {
		std::println("Game INIT!");

		if (!window.is_init_completed())
			return;

		textures = std::make_unique<assets::textures>();
		if (!textures->load())
			return;

		vao = generate_sample();
		vao->configure();

		ubo            = std::make_unique<gl_ubo>(sizeof(gl_vertex_uniform_data));
		shader_program = std::make_unique<gl_shader_program>();

		if (!shader_program->load())
			return;
		shader_program->set_uniform_block_slot("uniform_data", 0);

		init_completed = true;
	}

	[[nodiscard]] bool
	frame() {
		const auto now = clock::now();
		std::chrono::duration<float> duration = now - frame_time_point;
		delta_time = duration.count();
		frame_time_point = now;

		// std::println("Frame! {} / {} FPS", delta_time, 1.F / delta_time);

		if (glfwWindowShouldClose(window.get_window()) != 0) {
			glfwTerminate();
			return false;
		}
		glfwPollEvents();

		gl_vertex_uniform_data uniform_data{glm::mat4{1.F}};
		auto&                  transform = uniform_data.transform;
		transform                        = translate(transform, glm::vec3(0.75F, -0.75F, 0.F));
		transform                        = scale(transform, glm::vec3(0.5F, 0.5F, 0.5F));
		// transform = rotate(transform, glm::radians(90.F), glm::vec3(0.F, 0.F, 1.F));
		transform = rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(0.F, 0.F, 1.F));

		ubo->set_data(&uniform_data);

		gl_render_engine::clear();

		gl_render_engine::set_shader_program(shader_program);
		gl_render_engine::set_vao(vao);
		gl_render_engine::set_ubo(ubo, 0);
		gl_render_engine::set_texture(textures->get_atlas()->get_texture());

		gl_render_engine::render(static_cast<GLsizei>(vao->get_ebo().get_indices().size()));

		gl_render_engine::swap(window.get_window());

		// glfwSwapBuffers(window);?

		return true;
	}

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

	[[nodiscard]] bool
	is_init_completed() const { return init_completed; }
};
}

int
main() {
	game game{};

	if (!game.is_init_completed())
		return -1;

	while (game.frame()) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(10ms);
	}

	return 0;
}
