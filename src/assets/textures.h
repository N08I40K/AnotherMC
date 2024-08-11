//
// Created by n08i40k on 10.08.2024.
//

#ifndef TEXTURES_H
#define TEXTURES_H
#include "render/tex_atlas.h"

namespace assets {
class textures {
	std::unique_ptr<tex_atlas> atlas{nullptr};
	bool loaded{false};

public:
	textures() = default;

	[[nodiscard]] bool
	load();

	[[nodiscard]] tex_atlas*
	get_atlas() const { return atlas.get(); }

	[[nodiscard]] bool
	is_loaded() const { return loaded; }
};

using textures_ptr = std::unique_ptr<textures>;
}

#endif //TEXTURES_H
