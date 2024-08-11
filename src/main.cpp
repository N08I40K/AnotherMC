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

#include "objects/block_faces.h"
#include "render/gl_render_engine.h"
#include "render/gl_shader_program.h"
#include "render/gl_ubo.h"
#include "render/gl_window.h"

namespace {
struct gl_vertex_uniform_data {
	glm::mat4 transform;
	glm::mat4 projection;
};

class game {
public:
	using clock      = std::chrono::high_resolution_clock;
	using time_point = clock::time_point;

private:
	gl_window window;

	gl_shader_program_ptr shader_program;
	gl_vao_ptr            vao;
	gl_ubo_ptr            ubo;
	assets::textures_ptr  textures;

	bool init_completed{false};

	time_point frame_time_point{clock::now()};
	float      delta_time{0.F};

	[[nodiscard]] gl_vao_ptr
	generate_sample() const {
		gl_ubo ubo{sizeof(gl_vertex_uniform_data)};
		using namespace block_faces;

		auto vertices = ALL_VERTICES;
		// block_shade_uv_all(vertices.data(), glm::vec4(0.F, 0.F, 1.F, 1.F));
		block_shade_uv_all(vertices.data(), textures->get_atlas()->get_uvs().at("not.stone"));

		return std::make_unique<gl_vao>(vertices.data(), vertices.size(), FULL_BLOCK.data(), FULL_BLOCK.size());
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

		ubo            = std::make_unique<gl_ubo>(sizeof(gl_vertex_uniform_data));
		shader_program = std::make_unique<gl_shader_program>();

		if (!shader_program->load())
			return;
		shader_program->set_uniform_block_slot("uniform_data", 0);

		init_completed = true;
	}

	[[nodiscard]] bool
	frame() {
		const auto                   now      = clock::now();
		std::chrono::duration<float> duration = now - frame_time_point;
		delta_time                            = duration.count();
		frame_time_point                      = now;

		// std::println("Frame! {} / {} FPS", delta_time, 1.F / delta_time);

		if (glfwWindowShouldClose(window.get_window()) != 0) {
			glfwTerminate();
			return false;
		}
		glfwPollEvents();

		gl_vertex_uniform_data uniform_data{glm::mat4{1.F}, glm::mat4{1.F}};

		auto& transform = uniform_data.transform;
		transform       = scale(transform, glm::vec3(0.5F, 0.5F, 0.5F));
		transform       = rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(1.F, 0.F, 0.F));
		transform       = rotate(transform, static_cast<float>(glfwGetTime() * 2), glm::vec3(0.F, 1.F, 0.F));
		transform       = rotate(transform, static_cast<float>(glfwGetTime() * 3), glm::vec3(0.F, 0.F, 1.F));

		auto& projection = uniform_data.projection;

		int width;
		int height;
		glfwGetWindowSize(window.get_window(), &width, &height);
		const float aspect = static_cast<float>(width) / static_cast<float>(height);
		projection         = glm::ortho<float>(
			-1 * aspect,
			1 * aspect,
			-1,
			1,
			-1.F,
			1.F);

		ubo->set_data(&uniform_data);

		gl_render_engine::clear();

		gl_render_engine::set_face_cull(gl_render_engine::face_culling_type::BACK);
		gl_render_engine::set_winding_order(gl_render_engine::winding_order::CLOCK_WISE);

		gl_render_engine::set_shader_program(shader_program);
		gl_render_engine::set_vao(vao);
		gl_render_engine::set_ubo(ubo, 0);
		gl_render_engine::set_texture(textures->get_atlas()->get_texture());

		gl_render_engine::render(vao->get_ebo_size());

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
		std::this_thread::sleep_for(6ms);
	}

	return 0;
}
