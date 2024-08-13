//
// Created by n08i40k on 13.08.2024.
//

#include "registries.h"

const registry<block>&
registries::get_blocks() const { return blocks; }

const registry<entity>&
registries::get_entities() const { return entities; }
