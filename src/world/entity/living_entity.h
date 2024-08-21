//
// Created by n08i40k on 13.08.2024.
//

#ifndef LIVING_ENTITY_H
#define LIVING_ENTITY_H

#include "entity.h"

class living_entity : public entity {
	glm::vec3 velocity{0.F};

	float health{0.F};

public:
	living_entity() = default;

	living_entity(
		const glm::vec3& position,
		const glm::vec3& velocity);

	void
	read_state(
		std::istream& stream) override;

	void
	write_state(
		std::ostream& stream) override;

	[[nodiscard]] virtual float
	get_max_health() const = 0;

	[[nodiscard]] glm::vec3
	get_velocity() const { return velocity; }

	[[nodiscard]] float
	get_health() const { return health; }

	void
	tick() override {
		if (health <= 0.F)
			set_destroyed(true);
	}

	void
	kill() {
		set_destroyed(true);
		health = 0;
	}
};

#endif //LIVING_ENTITY_H
