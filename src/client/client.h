//
// Created by n08i40k on 12.08.2024.
//

#ifndef CLIENT_H
#define CLIENT_H
#include <array>

#include "../assets/textures.h"
#include "render/gl_camera.h"
#include "render/gl_shader_program.h"
#include "render/gl_ubo.h"
#include "render/gl_vao.h"
#include "render/gl_window.h"

struct gl_uniform_world_data {
	glm::mat4 projection{1.F};
	glm::mat4 view{1.F};
};

struct gl_uniform_model_data {
	glm::mat4 model{1.F};
};

class client {
public:
	enum class cursor_mode {
		NONE,
		CURSOR_DISABLED,
		CURSOR_ENABLED
	};

private:
	static client* instance;

	gl_window window;

	gl_shader_program_ptr shader_program;
	gl_vao_ptr            vao;
	gl_vao_ptr            vao2;

	gl_ubo_ptr world_ubo;
	gl_ubo_ptr model_ubo;

	gl_camera camera;

	// input
	std::array<bool, GLFW_KEY_LAST + 1> keys{false};

	cursor_mode mouse_cursor_mode{cursor_mode::NONE};
	glm::vec2   mouse_pos{0.F};

	assets::textures_ptr textures;

	// frame data
	GLfloat last_frame{};
	GLfloat delta_time{};

	bool init_completed{false};

	[[nodiscard]] gl_vao_ptr
	generate_sample(
		const char* name) const;

public:
	client();

	void
	set_mouse_cursor_mode(
		cursor_mode mouse_cursor_mode);

	void
	key_movement();

	[[nodiscard]] bool
	frame();

	static void
	key_callback(
		GLFWwindow*          window,
		int                  key,
		[[maybe_unused]] int scancode,
		int                  action,
		[[maybe_unused]] int mods);

	static void
	mouse_pos_callback(
		[[maybe_unused]] GLFWwindow* window,
		double                       x,
		double                       y);

	static void
	mouse_button_callback(
		GLFWwindow* window,
		int         button,
		int         action,
		int         mods);

	[[nodiscard]] bool
	is_init_completed() const;

	[[nodiscard]] assets::textures&
	get_textures() const { return *textures; }

	[[nodiscard]] static client*
	get_instance() { return instance; }

	void tick();
};

#endif //CLIENT_H
