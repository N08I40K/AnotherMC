//
// Created by n08i40k on 11.08.2024.
//

#include "gl_render_engine.h"

void
gl_render_engine::swap(
	GLFWwindow* window) { glfwSwapBuffers(window); }

void
gl_render_engine::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3F, 0.3F, 0.3F, 1.0f);
}

void
gl_render_engine::set_vao(
	const gl_vao_ptr& vao) {
	glBindVertexArray(vao->get_vao());
}

void
gl_render_engine::set_ubo(
	const gl_ubo_ptr& ubo,
	const GLuint      slot) {
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, ubo->get_ubo());
}

void
gl_render_engine::set_texture(
	const GLuint texture) { glBindTexture(GL_TEXTURE_2D, texture); }

void
gl_render_engine::set_shader_program(
	const gl_shader_program_ptr& shader_program) {
	glUseProgram(shader_program->get_shader_program());
}

void
gl_render_engine::set_face_cull(
	const face_culling_type type) {
	GLenum target;

	switch (type) {
	case face_culling_type::FRONT: {
		target = GL_FRONT;
		break;
	}
	case face_culling_type::BACK: {
		target = GL_BACK;
		break;
	}
	case face_culling_type::BOTH: {
		target = GL_FRONT_AND_BACK;
		break;
	}
	default: throw new std::runtime_error("Передан неверный тип face culling!");
	}

	glEnable(GL_CULL_FACE);
	glCullFace(target);
}

void
gl_render_engine::set_winding_order(
	const winding_order order) {
	GLenum target;

	switch (order) {
	case winding_order::CLOCK_WISE: {
		target = GL_CW;
		break;
	}
	case winding_order::COUNTER_CLOCK_WISE: {
		target = GL_CCW;
		break;
	}
	default: throw new std::runtime_error("Передан неверный тип winding order!");
	}

	glFrontFace(target);
}

void
gl_render_engine::set_depth_test(
	const bool enabled) {
	if (enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void
gl_render_engine::render(
	const GLsizei count) { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); }
