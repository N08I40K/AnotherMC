//
// Created by n08i40k on 14.08.2024.
//

#include "transfer_context.h"

#include <ostream>

transfer_context::transfer_context(
	const uint32_t id,
	std::vector<uint8_t>&& data) {
	std::ostream stream{std::_Uninitialized{}};
	bytes << id;
	stream << data;
}
