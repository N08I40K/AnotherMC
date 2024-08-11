//
// Created by n08i40k on 10.08.2024.
//

#include "textures.h"
#include <print>

bool
assets::textures::load() {
	if (loaded) {
		loaded = false;
		atlas = nullptr;
	}

	const auto textures_dir = std::filesystem::path{ASSERT_DIR} / "textures";

	if (!exists(textures_dir)) {
		std::println("Не удалось найти папку текстур! {}", textures_dir.string());
		return false;
	}

	if (!is_directory(textures_dir)) {
		std::println("Путь ведущий к папке текстур указывает не на папку! {}", textures_dir.string());
		return false;
	}

	raw_tex_atlas raw_tex_atlas{.entry_size = {16, 16}};

	glm::szvec2 raw_atlas_cursor{};
	bool        raw_state_flag = true;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(textures_dir)) {
		if (!entry.is_regular_file())
			continue;

		const auto& file_path = entry.path();
		if (file_path.extension() != ".png")
			continue;

		auto image = atlas_image::load(file_path);
		if (image == nullptr)
			return false;

		if (!raw_tex_atlas.emplace(raw_atlas_cursor, raw_state_flag, std::move(image)))
			return false;

		std::println("Успешно загружена текстура! {}", file_path.string());
	}

	atlas = raw_tex_atlas.compile();

	return loaded = true;
}
