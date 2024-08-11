//
// Created by n08i40k on 14.12.2023.
//

#ifndef ZENO_URPROJECT_REWRITE_VECTOR2_H
#define ZENO_URPROJECT_REWRITE_VECTOR2_H

#include <cassert>
#include <cstdint>
#include <numbers>
#include <utility>
#include <functional>

template <class VectorT, typename ReqValueT>
concept Vector2Like = requires(
	VectorT vector) {
			{ vector.x } -> std::convertible_to<ReqValueT>;
			{ vector.y } -> std::convertible_to<ReqValueT>;
		};

template <typename ValueT>
class vector2 {
	using FloatingPointT     = std::conditional_t<std::is_same_v<ValueT, double>, double, float>;
	using LongFloatingPointT = std::conditional_t<std::is_same_v<ValueT, double>, long double, double>;

public:
	ValueT x, y;

	vector2() : vector2(0, 0) {}

	template <typename IValueT>
	vector2(
		IValueT x,
		IValueT y) {
		this->x = static_cast<ValueT>(x);
		this->y = static_cast<ValueT>(y);
	}

	template <Vector2Like<ValueT> VectorT>
	explicit
	vector2(
		const VectorT& other_type) : vector2(
			                           other_type.x,
			                           other_type.y) {}

	constexpr ValueT&
	operator[](
		const uint8_t index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		default: return 0;
		}
	}

	constexpr ValueT
	operator[](
		const uint8_t index) const { return operator[](index); }

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] vector2
	rotate(
		const VectorT        center,
		const FloatingPointT degrees) const {
		// я не математик.
		// https://www.itcodar.com/c-plus-1/c-rotating-a-vector-around-a-certain-point.html

		FloatingPointT radians   = -degrees * (std::numbers::pi_v<float> / 180);
		FloatingPointT cos_theta = cos(radians);
		FloatingPointT sin_theta = sin(radians);

		return {
			((cos_theta * (x - center.x)) - (sin_theta * (y - center.y)) + center.x),
			((sin_theta * (x - center.x)) + (cos_theta * (y - center.y)) + center.y)
		};
	}

	void
	swap() {
		ValueT saved_y = y;
		y              = x;
		x              = saved_y;
	}

	[[nodiscard]] bool
	is_zero() const { return x == 0 && y == 0; }

	[[nodiscard]] FloatingPointT
	magnitude() const { return sqrt(static_cast<FloatingPointT>((x * x) + (y * y))); }

	template <Vector2Like<ValueT> VectorT>
	void
	for_each(
		const VectorT& to_input,
		const size_t   iterations,
		std::function<void(
			vector2)> callback) const {
		assert(iterations > 0);

		if (iterations == 1) {
			callback(to_input);
			return;
		}

		const vector2<float> from{*this};
		const vector2<float> to{to_input};

		float iteration_increment = from.distance(to) / static_cast<float>(iterations - 1);

		auto normalized_to = (to - from);
		normalized_to.normalize();

		const auto iteration_increment_vec2 = normalized_to * iteration_increment;

		auto current_path = from;
		for (size_t i{}; i < iterations; ++i) {
			callback(current_path.construct<vector2>());
			current_path += iteration_increment_vec2;
		}
	}

	void
	normalize() {
		if (const LongFloatingPointT sum = (x * x) + (y * y);
			sum > 0) {
			const LongFloatingPointT d = 1.0 / sqrtl(sum);
			x                          = d * x;
			y                          = d * y;
		}
		else
			x = 1.0;
	}

	[[nodiscard]] vector2
	normalise() const {
		if (const LongFloatingPointT sum = (x * x) + (y * y);
			sum > 0) {
			const LongFloatingPointT d = 1.0 / sqrtl(sum);
			return {d * x, d * y};
		}
		return {1, 0};
	}

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] vector2
	bound(
		VectorT min,
		VectorT max) const {
		return vector2{
			std::max<ValueT>(min.x, std::min<ValueT>(max.x, x)),
			std::max<ValueT>(min.y, std::min<ValueT>(max.y, y))
		};
	}

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] bool
	contains(
		const VectorT& min,
		const VectorT& max) const { return x <= max.x && y <= max.y && x >= min.x && y >= min.y; }

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] bool
	contains_polygon(
		const VectorT* polygon,
		const size_t   polygon_count) const {
		// Count of intersections
		int count = 0;

		// Iterate through each edge of the polygon
		for (int i = 0; i < polygon_count; i++) {
			const auto& p1 = polygon[i];
			// Ensure the last point connects to the first point
			const auto& p2 = polygon[(i + 1) % polygon_count];

			// Check if the point's y-coordinate is within the
			// edge's y-range and if the point is to the left of
			// the edge
			if ((y > std::min(p1.y, p2.y))
				&& (y <= std::max(p1.y, p2.y))
				&& (x <= std::max(p1.x, p2.x))) {
				// Calculate the x-coordinate of the
				// intersection of the edge with a horizontal
				// line through the point
				FloatingPointT x_intersect = ((y - p1.y)
							* (p2.x - p1.x)
							/ (p2.y - p1.y))
						+ p1.x;
				// If the edge is vertical or the point's
				// x-coordinate is less than or equal to the
				// intersection x-coordinate, increment count
				if (p1.x == p2.x || x <= x_intersect)
					count++;
			}
		}
		// If the number of intersections is odd, the point is
		// inside the polygon
		return count % 2 == 1;
	}

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] ValueT
	cross(
		const VectorT& b) const { return (x * b.y) - (y * b.x); }

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] ValueT
	cross(
		const VectorT& a,
		const VectorT& b) const { return (a - *this).cross(b - *this); }

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] ValueT
	dot(
		const VectorT& b) const { return (x * b.x) + (y * b.y); }

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] vector2
	steer(
		const VectorT& b,
		FloatingPointT delta) const {
		if (this->distance(b) < delta)
			return b;

		FloatingPointT theta = atan2(b.y - y, b.x - x);
		return vector2{x + (delta * cos(theta)), y + (delta * sin(theta))};
	}

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] vector2
	step_near(
		const VectorT& b,
		FloatingPointT delta) const {
		if ((distance(b) - delta) <= std::numeric_limits<FloatingPointT>::epsilon())
			return b;

		FloatingPointT theta = atan2(b.y - y, b.x - x);
		return {x + (delta * cos(theta)), y + (delta * sin(theta))};
	}

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] VectorT
	construct() const { return VectorT{x, y}; }

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] const VectorT&
	convert_type() const { return *reinterpret_cast<const VectorT*>(this); }

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	[[nodiscard]] vector2<IValueT>
	convert_value_type() const { return {x, y}; }

	template <Vector2Like<ValueT> VectorT>
	[[nodiscard]] FloatingPointT
	distance(
		const VectorT& b) const {
		return sqrt(static_cast<FloatingPointT>(((x - b.x) * (x - b.x)) + ((y - b.y) * (y - b.y))));
	}

	// Class operators
	template <Vector2Like<ValueT> VectorT>
	vector2&
	operator=(
		const VectorT& b) {
		x = b.x;
		y = b.y;

		return *this;
	}

	template <Vector2Like<ValueT> VectorT>
	vector2&
	operator+=(
		const VectorT& b) {
		x += b.x;
		y += b.y;

		return *this;
	}

	template <Vector2Like<ValueT> VectorT>
	vector2&
	operator-=(
		const VectorT& b) {
		x -= b.x;
		y -= b.y;

		return *this;
	}

	template <Vector2Like<ValueT> VectorT>
	vector2&
	operator*=(
		const VectorT& b) {
		x *= b.x;
		y *= b.y;

		return *this;
	}

	template <Vector2Like<ValueT> VectorT>
	vector2&
	operator/=(
		const VectorT& b) {
		x /= b.x;
		y /= b.y;

		return *this;
	}

	template <Vector2Like<ValueT> VectorT>
	vector2
	operator+(
		const VectorT& b) const { return {x + b.x, y + b.y}; }

	template <Vector2Like<ValueT> VectorT>
	vector2
	operator-(
		const VectorT& b) const { return {x - b.x, y - b.y}; }

	template <Vector2Like<ValueT> VectorT>
	vector2
	operator*(
		const VectorT& b) const { return {x * b.x, y * b.y}; }

	template <Vector2Like<ValueT> VectorT>
	vector2
	operator/(
		const VectorT& b) const { return {x / b.x, y / b.y}; }

	// Basic operators
	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2&
	operator=(
		IValueT b) {
		x = b;
		y = b;

		return *this;
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	bool
	operator==(
		vector2<IValueT> b) const { return x == b.x && y == b.y; }

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2&
	operator+=(
		IValueT b) {
		auto _b = static_cast<ValueT>(b);
		x += _b;
		y += _b;

		return *this;
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2&
	operator-=(
		IValueT b) {
		auto _b = static_cast<ValueT>(b);
		x -= _b;
		y -= _b;

		return *this;
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2&
	operator*=(
		IValueT b) {
		auto _b = static_cast<ValueT>(b);
		x *= _b;
		y *= _b;

		return *this;
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2&
	operator/=(
		IValueT b) {
		auto _b = static_cast<ValueT>(b);
		x /= _b;
		y /= _b;

		return *this;
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2
	operator+(
		IValueT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x + b, y + b};
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2
	operator-(
		IValueT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x - b, y - b};
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2
	operator*(
		IValueT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x * b, y * b};
	}

	template <typename IValueT>
		requires std::is_arithmetic_v<IValueT>
	vector2
	operator/(
		IValueT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x / b, y / b};
	}

	friend std::size_t
	hash_value(
		const vector2& obj) {
		std::size_t seed = 0x6EC3B789;
		seed ^= (seed << 6) + (seed >> 2) + 0x2D33D3F6 + hash_value(obj.x);
		seed ^= (seed << 6) + (seed >> 2) + 0x63387306 + hash_value(obj.y);
		return seed;
	}

	friend bool
	operator<(
		const vector2& lhs,
		const vector2& rhs) {
		if (lhs.x < rhs.x)
			return true;
		if (rhs.x < lhs.x)
			return false;
		return lhs.y < rhs.y;
	}
};

// ReSharper disable CppInconsistentNaming
using vector2f = vector2<float>;
using vector2d = vector2<double>;
using vector2i = vector2<int>;
using vector2sz = vector2<size_t>;
// ReSharper restore CppInconsistentNaming

#endif //ZENO_URPROJECT_REWRITE_VECTOR2_H
