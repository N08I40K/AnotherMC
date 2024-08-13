//
// Created by n08i40k on 13.08.2024.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "living_entity.h"

namespace entities {
class player final : public living_entity {
public:
	player() = default;

	player(
		const glm::vec3& position,
		const glm::vec3& velocity) : living_entity(position, velocity) {}

	// Должен переопределяться ТОЛЬКО финальными классами.
	entity*
	clone() override { return new player(*this); }

	[[nodiscard]] float
	get_max_health() const override { return 20.F; }
};
} // entities

#endif //PLAYER_H
