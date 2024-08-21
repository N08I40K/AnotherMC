//
// Created by n08i40k on 13.08.2024.
//

#ifndef REGISTRIES_H
#define REGISTRIES_H
#include "registry.h"

class block;
class entity;

class registries {
	registry<block>  blocks;
	registry<entity> entities;

public:
	registries();
	~registries();

	[[nodiscard]] const registry<block>&
	get_blocks() const;

	[[nodiscard]] const registry<entity>&
	get_entities() const;
};

#endif //REGISTRIES_H
