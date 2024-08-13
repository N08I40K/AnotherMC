//
// Created by n08i40k on 13.08.2024.
//

#include "registry/registry.h"
#include "world/block.h"
#include "world/block/simple_block.h"

registry<block>::registry() {
	add("stone", std::make_unique<simple_block>("stone"));
	add("grass", std::make_unique<simple_block>("grass"));
}
