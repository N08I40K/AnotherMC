//
// Created by n08i40k on 12.08.2024.
//

#ifndef BLOCK_H
#define BLOCK_H

#include <iosfwd>
#include "block_model.h"
#include "util/class_operations.h"

class block : public util::cloneable<block> {
public:
	explicit
	block() = default;

	[[nodiscard]] virtual block_model::type
	get_model_type() const = 0;

	virtual void
	read_state(
		std::istream& stream) {}

	virtual void
	write_state(
		std::ostream& stream) {}
};

#endif //BLOCK_H
