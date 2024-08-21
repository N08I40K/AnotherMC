//
// Created by n08i40k on 13.08.2024.
//

#include "registries.h"

#include "world/block.h"
#include "world/entity/entity.h"

registries::registries() = default;

registries::~registries() = default;

const registry<block>&
registries::get_blocks() const { return blocks; }

const registry<entity>&
registries::get_entities() const { return entities; }
