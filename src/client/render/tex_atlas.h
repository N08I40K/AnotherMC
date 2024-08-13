//
// Created by n08i40k on 10.08.2024.
//

#ifndef TEX_ATLAS_H
#define TEX_ATLAS_H
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec4.hpp>

#include "glm/vec2_add.hpp"

class atlas_image {
	std::string name;

	unsigned char* data;
	glm::szvec2    size;

	explicit
	atlas_image(
		std::string&&      name,
		unsigned char*     data,
		const glm::szvec2& size);

public:
	~atlas_image();

	static std::unique_ptr<atlas_image>
	load(
		const std::filesystem::path& file_path);

	[[nodiscard]] glm::szvec2
	get_size() const;

	[[nodiscard]] std::string
	get_name() const { return name; }

	[[nodiscard]] unsigned char*
	get_data() const { return data; }
};

struct atlas_entry {
	glm::szvec2 position;
	glm::szvec2 size;

	std::unique_ptr<atlas_image> image{nullptr};
};

class tex_atlas;

struct raw_tex_atlas {
	glm::szvec2 entry_size;
	glm::szvec2 size;

	std::vector<atlas_entry> entries;

	bool
	emplace(
		glm::szvec2&                   cursor,
		bool&                          state_flag,
		std::unique_ptr<atlas_image>&& image);

	[[nodiscard]] std::unique_ptr<tex_atlas>
	compile() const;
};

class tex_atlas {
	GLuint      texture{};
	glm::szvec2 size{};

	std::map<std::string, glm::vec4> uvs;

public:
	tex_atlas(
		std::map<std::string, glm::vec4>&& uvs,
		const unsigned char*               data,
		glm::szvec2                        size);

	tex_atlas(
		const tex_atlas& other) = delete;

	tex_atlas(
		tex_atlas&& other) noexcept : size(other.size) { std::swap(texture, other.texture); }

	tex_atlas&
	operator=(
		tex_atlas other) {
		std::swap(*this, other);
		return *this;
	}

	~tex_atlas();

	[[nodiscard]] GLuint
	get_texture() const { return texture; }

	[[nodiscard]] glm::szvec2
	get_size() const { return size; }

	[[nodiscard]] const std::map<std::string, glm::vec4>&
	get_uvs() const { return uvs; }
};

#endif //TEX_ATLAS_H
