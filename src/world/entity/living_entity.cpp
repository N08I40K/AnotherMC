//
// Created by n08i40k on 13.08.2024.
//

#include "living_entity.h"

living_entity::living_entity(
	const glm::vec3& position,
	const glm::vec3& velocity)
	: entity(position)
	, velocity(velocity) {}

void
living_entity::read_state(
	std::istream& stream) {
	entity::read_state(stream);

	stream >> velocity.x >> velocity.y >> velocity.z;
	stream >> health;
}

void
living_entity::write_state(
	std::ostream& stream) {
	entity::write_state(stream);

	stream << velocity.x << velocity.y << velocity.z;
	stream << health;
}
