//
// Created by n08i40k on 11.08.2024.
//

#include "gl_shader_program.h"

#include <fstream>
#include <print>
#include <array>
#include <vector>

std::optional<std::vector<char> >
gl_shader_program::read_shader_file(
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
	if (!read_stream.is_open()) {
		std::println("Не удалось открыть файл шейдера! {}", file_path.string());
		return std::nullopt;
	}

	const auto size = read_stream.tellg();
	read_stream.seekg(0, std::ios::beg);

	std::vector<GLchar> contents(size);
	if (!read_stream.read(contents.data(), size)) {
		std::println("Не удалось прочитать файл шейдера! {}", file_path.string());
		return std::nullopt;
	}

	return std::move(contents);
}

// ReSharper disable CppDFAConstantParameter
bool
gl_shader_program::load_shader(
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

gl_shader_program::~gl_shader_program() {
	if (shader_program != 0)
		glDeleteProgram(shader_program);
}

bool
gl_shader_program::load() {
	if (loaded) {
		loaded = false;

		glDeleteProgram(shader_program);
		shader_program = 0;
	}

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

	return loaded = true;
}

void
gl_shader_program::set_uniform_block_slot(
	const char*  name,
	const GLuint slot) const {
	GLuint idx = glGetUniformBlockIndex(shader_program, name);
	glUniformBlockBinding(shader_program, idx, slot);
}
