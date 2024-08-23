//
// Created by n08i40k on 12.08.2024.
//

#include "client.h"

#include <glm/ext/matrix_transform.hpp>

#include "objects/block_faces.h"
#include "render/gl_render_engine.h"

#include <print>
#include <thread>
#include <glm/ext/matrix_clip_space.hpp>

#include "network/transfer_context.h"
#include "network/client/client_session.h"
#include "world/world.h"
#include "world/entity/entity.h"
#include "world/entity/player.h"

#include <s2c_spawn.pb.h>

#include "util/bstream.h"
#include "util/entity.h"

gl_vao_ptr
client::generate_sample(
	const char* name) const {
	using namespace block_faces;

	auto vertices = ALL_VERTICES;

	auto        uv     = textures->get_atlas()->get_uvs();
	const auto& front  = uv.at("front");
	const auto& back   = uv.at("back");
	const auto& top    = uv.at("top");
	const auto& bottom = uv.at("bottom");
	const auto& right  = uv.at("right");
	const auto& left   = uv.at("left");

	block_shade_uv(vertices.data(), &front, &back, &top, &bottom, &right, &left);

	return std::make_unique<gl_vao>(vertices.data(), vertices.size(), FULL_BLOCK.data(), FULL_BLOCK.size());
}

client::client()
	: window(key_callback, mouse_pos_callback, mouse_button_callback) {
	instance = this;

	std::println("Game INIT!");

	if (!window.is_init_completed())
		return;

	textures = std::make_unique<assets::textures>();
	if (!textures->load())
		return;

	world_ubo      = std::make_unique<gl_ubo>(sizeof(gl_uniform_world_data));
	model_ubo      = std::make_unique<gl_ubo>(sizeof(gl_uniform_model_data));
	shader_program = std::make_unique<gl_shader_program>();

	if (!shader_program->load())
		return;
	shader_program->set_uniform_block_slot("world_data", 0);
	shader_program->set_uniform_block_slot("model_data", 1);

	set_mouse_cursor_mode(cursor_mode::CURSOR_DISABLED);

	std::println("Client init! Wait 3 sec for server initialization...");
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::println("Connecting...");
	network = std::make_unique<network::client_session>("127.0.0.1:20512");
	network->start_thread();
	std::println("Connected?");

	world = std::make_unique<::world>();

	init_completed = true;
}

client::~client() = default;

void
client::set_mouse_cursor_mode(
	const cursor_mode mouse_cursor_mode) {
	if (this->mouse_cursor_mode == mouse_cursor_mode)
		return;

	this->mouse_cursor_mode = mouse_cursor_mode;

	int cursor_state;
	switch (mouse_cursor_mode) {
	case cursor_mode::CURSOR_DISABLED: {
		cursor_state = GLFW_CURSOR_DISABLED;
		break;
	}
	case cursor_mode::CURSOR_ENABLED: {
		cursor_state = GLFW_CURSOR_NORMAL;
		break;
	}
	case cursor_mode::NONE:
	default: throw std::runtime_error("Передан неизвестный режим курсора мыши!");
	}

	glfwSetInputMode(window.get_window(), GLFW_CURSOR, cursor_state);
}

void
client::key_movement() {
	const GLfloat camera_speed = 2.F * delta_time;

	const auto& front = camera.get_direction();
	const auto& up    = gl_camera::get_up();

	glm::vec3 move{0.F};

	if (keys[GLFW_KEY_W])
		move += front;
	if (keys[GLFW_KEY_S])
		move -= front;

	if (keys[GLFW_KEY_A])
		move -= normalize(cross(camera.get_direction(), up));
	if (keys[GLFW_KEY_D])
		move += normalize(cross(camera.get_direction(), up));

	move.y = 0.F;

	if (keys[GLFW_KEY_SPACE])
		move += up;
	if (keys[GLFW_KEY_LEFT_SHIFT])
		move -= up;

	if (move != glm::vec3{0.F})
		camera.set_position(camera.get_position() + (normalize(move) * camera_speed));
}

bool
client::frame() {
	const auto now = static_cast<GLfloat>(glfwGetTime());

	if (last_frame == 0.F)
		last_frame = now;

	delta_time = now - last_frame;
	last_frame = now;

	// std::println("Frame! {} / {} FPS", delta_time, 1.F / delta_time);

	if (glfwWindowShouldClose(window.get_window()) != 0) {
		glfwTerminate();
		return false;
	}
	glfwPollEvents();

	key_movement();

	// define perspective and view matrices
	{
		int width;
		int height;
		glfwGetWindowSize(window.get_window(), &width, &height);
		const float aspect = static_cast<float>(width) / static_cast<float>(height);

		gl_uniform_world_data world_data{
			.projection = glm::perspective(45.F, aspect, .1F, 100.F),
			.view = camera.get_view()
		};

		world_ubo->set_data(&world_data);
		gl_render_engine::set_ubo(world_ubo, 0);
	}

	gl_render_engine::clear();

	gl_render_engine::set_face_cull(gl_render_engine::face_culling_type::FRONT);
	gl_render_engine::set_winding_order(gl_render_engine::winding_order::CLOCK_WISE);
	gl_render_engine::set_depth_test(true);

	gl_render_engine::set_shader_program(shader_program);

	gl_render_engine::set_texture(textures->get_atlas()->get_texture());

	//
	{
		gl_render_engine::set_vao(vao);

		gl_uniform_model_data model_data{};
		auto&                 transform = model_data.model;
		transform                       = scale(transform, glm::vec3(0.5F, 0.5F, 0.5F));
		// transform = rotate(transform, static_cast<float>(glfwGetTime() * 0.5), glm::vec3(0.F, 1.F, 0.F));
		// transform = rotate(transform, static_cast<float>(glfwGetTime() * 0.75), glm::vec3(0.F, 0.F, 1.F));
		model_ubo->set_data(&model_data);

		gl_render_engine::set_ubo(model_ubo, 1);
		gl_render_engine::render(vao->get_ebo_size());
	}

	//
	{
		gl_render_engine::set_vao(vao2);

		gl_uniform_model_data model_data{};
		auto&                 transform = model_data.model;
		transform                       = scale(transform, glm::vec3(0.5F, 0.5F, 0.5F));
		transform                       = translate(transform, glm::vec3(2.F, 0.F, 0.f));

		// transform = rotate(transform, static_cast<float>(glfwGetTime() * 0.5), glm::vec3(0.F, 1.F, 0.F));
		// transform = rotate(transform, static_cast<float>(glfwGetTime() * 0.75), glm::vec3(0.F, 0.F, 1.F));
		model_ubo->set_data(&model_data);

		gl_render_engine::set_ubo(model_ubo, 1);
		gl_render_engine::render(vao2->get_ebo_size());
	}

	auto position = camera.get_position();

	//
	{
		gl_render_engine::set_vao(vao2);

		gl_uniform_model_data model_data{};
		auto&                 transform = model_data.model;
		transform                       = scale(transform, glm::vec3(0.5F, 0.5F, 0.5F));
		transform                       = translate(transform, glm::vec3(2.F, 2.F, -2.f));

		// transform = rotate(transform, static_cast<float>(glfwGetTime() * 0.5), glm::vec3(0.F, 1.F, 0.F));
		// transform = rotate(transform, static_cast<float>(glfwGetTime() * 0.75), glm::vec3(0.F, 0.F, 1.F));
		model_ubo->set_data(&model_data);

		gl_render_engine::set_ubo(model_ubo, 1);
		gl_render_engine::render(vao2->get_ebo_size());
	}

	gl_render_engine::swap(window.get_window());

	return true;
}

void
client::key_callback(
	GLFWwindow* window,
	const int   key,
	const int   scancode,
	const int   action,
	const int   mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key < 0)
		return;
	instance->keys[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
}

void
client::mouse_pos_callback(
	GLFWwindow*  window,
	const double x,
	const double y) {
	auto& prev_mouse_pos = instance->mouse_pos;

	const glm::vec2 mouse_pos{static_cast<float>(x), static_cast<float>(y)};

	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);

	glm::vec2 delta = {
		(mouse_pos.x - prev_mouse_pos.x),
		(prev_mouse_pos.y - mouse_pos.y)
	};

	prev_mouse_pos = mouse_pos;

	if (instance->mouse_cursor_mode == cursor_mode::CURSOR_DISABLED)
		instance->camera.handle_mouse_move(delta);
}

void
client::mouse_button_callback(
	GLFWwindow* window,
	int         button,
	int         action,
	int         mods) {}

bool
client::is_init_completed() const { return init_completed; }

void
client::tick() {
	if (network == nullptr)
		return;

	for (auto  packets = network->get_received_packets();
	     auto& packet : packets) {
		switch (auto [packet_id, packet_data] = packet->take_data(); packet_id) {
		case network::packet_id::S2C_SPAWN: {
			s2c_spawn spawn_packet;
			spawn_packet.ParseFromString(packet_data);

			player = dynamic_cast<entities::player*>(
				game::get_instance().get_registries().get_entities()["player"]->clone());
			utils::entity::parse_data(player, spawn_packet.data());

			std::println("spawn");
			break;
		}
		default: break;
		}
	}
}

client* client::instance = nullptr;
