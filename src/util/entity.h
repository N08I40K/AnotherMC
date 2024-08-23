//
// Created by n08i40k on 23.08.2024.
//

#ifndef ENTITY_UTILS_H
#define ENTITY_UTILS_H
#include <string>

class entity;
class entity_data;

namespace utils::entity {
void
parse_data(
	::entity*          entity,
	const entity_data& data);

entity_data
serialize_data(
	::entity* entity);
}

#endif //ENTITY_UTILS_H
