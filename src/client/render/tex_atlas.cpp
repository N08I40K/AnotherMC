//
// Created by n08i40k on 10.08.2024.
//

#include "tex_atlas.h"
#include <cstddef>
#include <print>
#include <SOIL/SOIL.h>
#include <glm/vec2.hpp>

#include <range/v3/algorithm/any_of.hpp>

atlas_image::atlas_image(
	std::string&&      name,
	unsigned char*     data,
	const glm::szvec2& size)
	: name(std::move(name))
	, data(data)
	, size(size) {}

atlas_image::~atlas_image() {
	if (data != nullptr)
		SOIL_free_image_data(data);
}

std::unique_ptr<atlas_image>
atlas_image::load(
	const std::filesystem::path& file_path) {
	int   soil_width;
	int   soil_height;
	auto* data = SOIL_load_image(file_path.string().c_str(), &soil_width, &soil_height, nullptr, SOIL_LOAD_RGBA);
	if (data == nullptr) {
		std::println("Не удалось загрузить текстуру! {}", file_path.string());
		return nullptr;
	}

	glm::szvec2 size{soil_width, soil_height};

	// invert y
	std::vector<uint32_t> backup(size.x * size.y);
	memcpy(backup.data(), data, (size.x * size.y) << 2);

	for (size_t y{}; y < size.y; ++y) {
		memcpy(&data[(size.x * (size.y - y - 1)) << 2],
		       &backup[size.x * y],
		       size.x << 2);
	}
	// invert y

	const auto file_name_with_ext = file_path.filename().string();
	return std::unique_ptr<atlas_image>(new atlas_image{
		file_name_with_ext.substr(0, file_name_with_ext.size() - 4), data, size
	});
}

glm::szvec2
atlas_image::get_size() const { return size; }

bool
raw_tex_atlas::emplace(
	glm::szvec2&                   cursor,
	bool&                          state_flag,
	std::unique_ptr<atlas_image>&& image) {
	if (image->get_size() != entry_size) {
		std::println(
			"Несоответствие требуемого размера текстуры и действительного! {}\n"
			"Требуемый {}x{}px.\n"
			"Действительный {}x{}px.",
			image->get_name(),
			entry_size.x,
			entry_size.y,
			image->get_size().x,
			image->get_size().y);
		return false;
	}

	if (ranges::any_of(entries,
	                   [&image](
                   const atlas_entry& entry) {
		                   return entry.image->get_name() == image->get_name();
	                   })) {
		std::println("Найден дубликат текстуры!\n"
		             "Название {}\n",
		             image->get_name());
		return false;
	}

retry:
	if (size.x < cursor.x + entry_size.x
		|| size.y < cursor.y + entry_size.y) {
		if (size.x < size.y) {
			size.x     = cursor.x + entry_size.x;
			cursor.y   = 0;
			state_flag = false;
		}
		else {
			size.y     = cursor.y + entry_size.y;
			cursor.x   = 0;
			state_flag = true;
		}
		goto retry;
	}

	entries.emplace_back(cursor, entry_size, std::move(image));
	if (state_flag)
		cursor.x += entry_size.x;
	else
		cursor.y += entry_size.y;

	return true;
}

void
copy_data(
	uint32_t*         target,
	const glm::szvec2 target_size,
	const glm::szvec2 target_position,
	const uint32_t*   source,
	const glm::szvec2 source_size) {
	for (size_t y{}; y < source_size.y; ++y) {
		memcpy(&target[((target_size.x * (y + target_position.y)) + target_position.x)],
		       &source[y * source_size.x],
		       source_size.x * 4);
	}
}

std::unique_ptr<tex_atlas>
raw_tex_atlas::compile() const {
	auto*                            buffer = new unsigned char[size.x * size.y * 4]{};
	std::map<std::string, glm::vec4> uvs;

	const glm::vec4 divider{size.x, size.y, size.x, size.y};

	for (const auto& [entry_position, entry_size, entry_image] : entries) {
		copy_data(reinterpret_cast<uint32_t*>(buffer),
		          size,
		          entry_position,
		          reinterpret_cast<const uint32_t*>(entry_image->get_data()),
		          entry_size);
		uvs.emplace(entry_image->get_name(),
		            glm::vec4(entry_position.x,
		                      entry_position.y,
		                      entry_position.x + entry_size.x,
		                      entry_position.y + entry_size.y) / divider);
	}

	auto atlas = std::make_unique<tex_atlas>(std::move(uvs), buffer, size);

	delete[] buffer;

	return atlas;
}

tex_atlas::tex_atlas(
	std::map<std::string, glm::vec4>&& uvs,
	const unsigned char*               data,
	const glm::szvec2                  size)
	: uvs(std::move(uvs)) {
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	// Warp
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Clamp
	std::array border_color{0.5F, 0.F, 1.F, 1.F};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color.data());

	// Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); // если далеко
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);				 // если близко

	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             GL_RGBA,
	             static_cast<int>(size.x),
	             static_cast<int>(size.y),
	             0,
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

tex_atlas::~tex_atlas() {
	if (texture != 0)
		glDeleteTextures(1, &texture);
}
