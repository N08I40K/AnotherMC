//
// Created by n08i40k on 13.08.2024.
//

#include "entity.h"

#include "game.h"

#include "util/bstream.h"

entity::entity(
	const glm::vec3& position): position(position) {}

entity::~entity() = default;

void
entity::tick() {}

void
entity::parse_data(
	stdn::brstream& stream) {
	std::array<uuids::uuid::value_type, 16> uuid_data{};
	stream >> uuid_data;
	uuid = uuids::uuid(uuid_data);

	stream >> position.x >> position.y >> position.z;
	stream >> look.x >> look.y;
}

void
entity::serialize_data(
	stdn::bwstream& stream) {
	stream << uuid.as_bytes();
	stream << position.x << position.y << position.z;
	stream << look.x << look.y;
}

glm::vec3
entity::get_position() const { return position; }

void
entity::set_position(
	const glm::vec3& position) { this->position = position; }

bool
entity::is_destroyed() const { return destroyed; }

void
entity::set_destroyed(
	const bool destroyed) {
	if (game::get_instance().is_server_tick())
		this->destroyed = destroyed;
}
