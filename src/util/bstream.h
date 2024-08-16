//
// Created by n08i40k on 16.08.2024.
//

#ifndef BSTREAM_H
#define BSTREAM_H
#include <stdexcept>
#include <vector>
#include "type_traits_n.h"

namespace stdn {
template <class T>
concept brstream_readable_container = requires(
	const T& t) {
			{ t.begin() };
			{ t.end() };
			{ t.size() } -> std::convertible_to<size_t>;
		};

template <class T>
concept bwstream_writeable_container = requires(
	T& t) {
			{ t.begin() };
		};

class brstream {
	const std::vector<uint8_t>& buffer;
	size_t                      read_pos{};

public:
	explicit
	brstream(
		const std::vector<uint8_t>& buffer) : buffer(buffer) {}

	template <bwstream_writeable_container T>
	brstream&
	operator>>(
		T& val) {
		if (read_pos + sizeof(size_t) > buffer.size())
			throw std::out_of_range("brstream::operator>>");

		size_t size;
		*this >> size;

		const auto binary_size = size * sizeof(*val.begin());
		if (read_pos + binary_size > buffer.size())
			throw std::out_of_range("brstream::operator>>");

		if constexpr (stdn::has_resize_v<T>)
			val.resize(size);
		else if constexpr (stdn::has_size_v<T>) {
			if (val.size() != size)
				throw std::out_of_range("brstream::operator>>");
		}
		else
			static_assert("Оператор записи нельзя использовать на классах без функции size или resize!");

		memcpy(&(*val.begin()), &buffer[read_pos], binary_size);
		read_pos += binary_size;

		return *this;
	}

	template <typename T>
	brstream&
	operator>>(
		T& val) {
		if (read_pos + sizeof(T) > buffer.size())
			throw std::out_of_range("brstream::operator>>");

		memcpy(&val, &buffer[read_pos], sizeof(T));
		read_pos += sizeof(T);

		return *this;
	}
};

class bwstream {
	std::vector<uint8_t> data;

public:
	bwstream()  = default;
	~bwstream() = default;

	template <brstream_readable_container T>
	bwstream&
	operator<<(
		const T& val) {
		*this << val.size();

		const auto old_size    = data.size();
		const auto binary_size = val.size() * sizeof(*val.begin());

		data.resize(old_size + binary_size);
		memcpy(&data[old_size], &(*val.begin()), binary_size);

		return *this;
	}

	template <typename T>
	bwstream&
	operator<<(
		const T& val) {
		const auto old_size = data.size();

		data.resize(old_size + sizeof(T));
		memcpy(&data[old_size], &val, sizeof(T));

		return *this;
	}

	[[nodiscard]] const std::vector<uint8_t>&
	get_buffer() const { return data; }

	[[nodiscard]] std::vector<uint8_t>
	take_result() { return std::move(data); }
};
}

#endif //BSTREAM_H
