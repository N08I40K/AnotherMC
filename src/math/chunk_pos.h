//
// Created by n08i40k on 12.08.2024.
//

#ifndef CHUNK_POS_H
#define CHUNK_POS_H
#include <boost/functional/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct chunk_pos {
	glm::ivec2 position;

	chunk_pos(
		const int x,
		const int z) : position{x, z} {}

	constexpr static size_t max_xz{16};
	constexpr static size_t max_y{256};

	[[nodiscard]] glm::ivec3
	as_world_pos() const { return {position.x * max_xz, 0, position.y * max_xz}; }

	friend bool
	operator==(
		const chunk_pos& lhs,
		const chunk_pos& rhs) { return lhs.position == rhs.position; }

	friend bool
	operator!=(
		const chunk_pos& lhs,
		const chunk_pos& rhs) { return !(lhs == rhs); }
};

template <>
struct std::hash<chunk_pos> {
	inline std::size_t
	operator()(
		const chunk_pos& obj) const noexcept {
		std::size_t seed = 0x203688D0;
		boost::hash_combine(seed, obj.position.x);
		boost::hash_combine(seed, obj.position.y);
		return seed;
	}
};

#endif //CHUNK_POS_H
