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
concept brstream_readable_buffer = requires(
	const T& t) {
			{ sizeof(*t.data()) == sizeof(uint8_t) };
			{ t.size() } -> std::convertible_to<size_t>;
		};

template <class T>
concept brstream_readable_container = requires(
	const T& t) {
			{ t.begin() };
			{ t.end() };
			{ t.size() } -> std::convertible_to<size_t>;
		};

template <class T>
concept bwstream_writeable_buffer = requires(
	T& t) {
			{ t.data() };
		};

template <class T>
concept bwstream_writeable_container = requires(
	T& t) {
			{ t.begin() };
		};

class brstream {
	const uint8_t* buffer;
	const size_t   buffer_size;
	size_t         read_pos{};

public:
	template <brstream_readable_buffer BufferT>
	explicit
	brstream(
		const BufferT& buffer)
		: buffer(reinterpret_cast<const uint8_t*>(buffer.data()))
		, buffer_size(buffer.size()) {}

	template <bwstream_writeable_container T>
	brstream&
	operator>>(
		T& val) {
		if (read_pos + sizeof(size_t) > buffer_size)
			throw std::out_of_range("brstream::operator>>");

		size_t size;
		*this >> size;

		const auto binary_size = size * sizeof(*val.begin());
		if (read_pos + binary_size > buffer_size)
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
		if (read_pos + sizeof(T) > buffer_size)
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

	template <bwstream_writeable_buffer BufferT>
	[[nodiscard]] BufferT
	take_result() {
		static_assert(stdn::has_resize_v<BufferT>);
		BufferT buffer;
		static_assert(sizeof(*buffer.data()) == sizeof(*data.data()));

		buffer.resize(data.size());
		memcpy(buffer.data(), data.data(), data.size());

		return std::move(buffer);
	}
};
}

#endif //BSTREAM_H
