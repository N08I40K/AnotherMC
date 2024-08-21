//
// Created by n08i40k on 12.08.2024.
//

#include "block.h"

#include "client/model/full_block.h"
#include "../client/client.h"
//
// models::full_block
// block::get_model() const {
// 	if (get_render_mode() == render_mode::OWN)
// 		throw std::runtime_error("block::get_model() | Блок имеет собственный рендер!");
//
// 	const auto& uvs = client::get_instance()->get_textures().get_atlas()->get_uvs();
//
// 	const auto& name = get_name();
// 	switch (get_render_mode()) {
// 	case render_mode::SIMPLE: return models::full_block{&uvs.at(name)};
// 	case render_mode::FRONT: return models::full_block{&uvs.at(name + "_front"), &uvs.at(name)};
// 	case render_mode::FRONT_TOP_DOWN: return models::full_block{
// 			&uvs.at(name + "_front"),
// 			&uvs.at(name + "_top"),
// 			&uvs.at(name + "_down"),
// 			&uvs.at(name)
// 		};
// 	default: throw std::runtime_error("block::get_model() | Блок имеет некорректный режим рендера!");
// 	}
// }
