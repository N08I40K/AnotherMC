//
// Created by n08i40k on 11.08.2024.
//

#ifndef GL_RENDER_ENGINE_H
#define GL_RENDER_ENGINE_H
#include "gl_shader_program.h"
#include "gl_ubo.h"
#include "gl_vao.h"

#include <GLFW/glfw3.h>

class gl_render_engine {
public:
	enum class face_culling_type {
		FRONT,
		BACK,
		BOTH
	};

	enum class winding_order {
		CLOCK_WISE,
		COUNTER_CLOCK_WISE
	};

	gl_render_engine()  = delete;
	~gl_render_engine() = delete;

	static void
	swap(
		GLFWwindow* window);

	static void
	clear();

	static void
	set_vao(
		const gl_vao_ptr& vao);

	static void
	set_ubo(
		const gl_ubo_ptr& ubo,
		GLuint            slot);

	static void
	set_texture(
		GLuint texture);

	static void
	set_shader_program(
		const gl_shader_program_ptr& shader_program);

	static void
	set_face_cull(
		face_culling_type type);

	static void
	set_winding_order(
		winding_order order);

	static void
	set_depth_test(
		bool enabled);

	static void
	render(
		GLsizei count);
};

#endif //GL_RENDER_ENGINE_H
