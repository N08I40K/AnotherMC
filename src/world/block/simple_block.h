//
// Created by n08i40k on 13.08.2024.
//

#ifndef DEFAULT_BLOCK_H
#define DEFAULT_BLOCK_H

#include <string>
#include <utility>

#include "world/block.h"

class simple_block final : public block {
	std::string name;

public:
	explicit
	simple_block(
		std::string name) : name(std::move(name)) {}

	[[nodiscard]] block_model::type
	get_model_type() const override;

	[[nodiscard]] block*
	clone() const override { return new simple_block(*this); }

	void
	set_name(
		const std::string& name) { this->name = name; }
};

#endif //DEFAULT_BLOCK_H
