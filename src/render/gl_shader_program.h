//
// Created by n08i40k on 11.08.2024.
//

#ifndef GL_SHADER_PROGRAM_H
#define GL_SHADER_PROGRAM_H
#include <filesystem>
#include <optional>
#include <GL/glew.h>

class gl_shader_program {
	GLuint shader_program{};
	bool   loaded{false};

	static std::optional<std::vector<char> >
	read_shader_file(
		const std::filesystem::path& file_path);

	static bool
	load_shader(
		GLuint&                      shader,
		GLsizei                      count,
		GLenum                       type,
		const std::filesystem::path& path);

public:
	~gl_shader_program();

	bool
	load();

	void
	set_uniform_block_slot(
		const char* name,
		GLuint      slot) const;

	[[nodiscard]] GLuint
	get_shader_program() const { return shader_program; }

	[[nodiscard]] bool
	is_loaded() const { return loaded; }
};

using gl_shader_program_ptr = std::unique_ptr<gl_shader_program>;

#endif //GL_SHADER_PROGRAM_H
