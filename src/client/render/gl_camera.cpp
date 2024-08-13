//
// Created by n08i40k on 12.08.2024.
//

#include "gl_camera.h"

#include <glm/ext/matrix_transform.hpp>

void
gl_camera::handle_mouse_move(
	const glm::vec2& delta) {
	set_yaw(yaw + (delta.x * sensitivity));
	set_pitch(pitch + (delta.y * sensitivity));
}

glm::mat4
gl_camera::get_view() {
	const GLfloat pitch_rad = glm::radians(pitch);
	const GLfloat yaw_rad   = glm::radians(yaw);

	direction.x = cos(yaw_rad) * cos(pitch_rad);
	direction.y = sin(pitch_rad);
	direction.z = sin(yaw_rad) * cos(pitch_rad);
	direction   = glm::normalize(direction);

	return lookAt(position, position + direction, up);
}
