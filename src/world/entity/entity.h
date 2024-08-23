//
// Created by n08i40k on 13.08.2024.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <uuid.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace stdn {
class bwstream;
class brstream;
}

class entity {
	uuids::uuid uuid;

	glm::vec3 position{0.F};
	glm::vec2 look{0.F};

	bool destroyed{false};

public:
	entity() = default;

	entity(
		entity&& other) noexcept {
		uuid = uuids::uuid_system_generator{}();

		position = other.position;
		look     = other.look;

		destroyed = false;
	}

	explicit
	entity(
		const glm::vec3& position);

	virtual ~entity();

	virtual void
	tick();

	virtual entity*
	clone() = 0;

	virtual void
	parse_data(
		stdn::brstream& stream);

	virtual void
	serialize_data(
		stdn::bwstream& stream);

	[[nodiscard]] glm::vec3
	get_position() const;

	void
	set_position(
		const glm::vec3& position);

	[[nodiscard]] bool
	is_destroyed() const;

	void
	set_destroyed(
		bool destroyed);

	[[nodiscard]] const uuids::uuid&
	get_uuid() const { return uuid; }
};

#endif //ENTITY_H
