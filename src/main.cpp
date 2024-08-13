#include <array>
#include <thread>
#include <chrono>
#include <print>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assets/textures.h"
#include "render/gl_vao.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "client/client.h"
#include "objects/block_faces.h"
#include "render/gl_camera.h"
#include "render/gl_render_engine.h"
#include "render/gl_shader_program.h"
#include "render/gl_ubo.h"
#include "render/gl_window.h"


int
main() {
	client game{};

	if (!game.is_init_completed())
		return -1;

	while (game.frame()) {
		using namespace std::chrono_literals;
		// std::this_thread::sleep_for(1ms);
	}

	return 0;
}
