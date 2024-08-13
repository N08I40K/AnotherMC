//
// Created by n08i40k on 12.08.2024.
//

#ifndef GL_CAMERA_H
#define GL_CAMERA_H
#include <algorithm>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "gl_window.h"

class gl_camera {
	glm::vec3 position{0.F, 0.F, -3.F};
	glm::vec3 direction{0.F};

	constexpr static glm::vec3 up{0.F, 1.F, 0.F};

	GLfloat pitch{0.F};
	GLfloat yaw{-90.F};

	GLfloat sensitivity{0.1F};

public:
	gl_camera()  = default;
	~gl_camera() = default;

	void
	handle_mouse_move(
		const glm::vec2& delta);

	[[nodiscard]] glm::mat4
	get_view();

	[[nodiscard]] glm::vec3
	get_position() const { return position; }

	void
	set_position(
		const glm::vec3& position) { this->position = position; }

	[[nodiscard]] glm::vec3
	get_direction() const { return direction; }

	[[nodiscard]] static glm::vec3
	get_up() { return up; }

	[[nodiscard]] GLfloat
	get_pitch() const { return pitch; }

	void
	set_pitch(
		const GLfloat pitch) {
		this->pitch = std::clamp(pitch, -89.F, 89.F);
	}

	[[nodiscard]] GLfloat
	get_yaw() const { return yaw; }

	void
	set_yaw(
		const GLfloat yaw) { this->yaw = yaw; }

	[[nodiscard]] GLfloat
	get_sensitivity() const { return sensitivity; }

	void
	set_sensitivity(
		const GLfloat sensitivity) { this->sensitivity = sensitivity; }
};

#endif //GL_CAMERA_H
