//
// Created by n08i40k on 13.08.2024.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <istream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class entity {
	glm::vec3 position{0.F};
	glm::vec2 look{0.F};

	bool destroyed{false};

public:
	entity() = default;

	explicit
	entity(
		const glm::vec3& position);

	virtual ~entity();

	virtual void
	tick();

	virtual entity*
	clone() = 0;

	virtual void
	read_state(
		std::istream& stream);

	virtual void
	write_state(
		std::ostream& stream);

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
};

#endif //ENTITY_H
