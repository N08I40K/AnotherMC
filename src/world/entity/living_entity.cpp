//
// Created by n08i40k on 13.08.2024.
//

#include "living_entity.h"
#include "util/bstream.h"

living_entity::living_entity(
	const glm::vec3& position,
	const glm::vec3& velocity)
	: entity(position)
	, velocity(velocity) {}

void
living_entity::parse_data(
	stdn::brstream& stream) {
	entity::parse_data(stream);

	stream >> velocity.x >> velocity.y >> velocity.z;
	stream >> health;
}

void
living_entity::serialize_data(
	stdn::bwstream& stream) {
	entity::serialize_data(stream);

	stream << velocity.x << velocity.y << velocity.z;
	stream << health;
}
