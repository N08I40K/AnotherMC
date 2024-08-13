//
// Created by n08i40k on 12.08.2024.
//

#ifndef CHUNK_H
#define CHUNK_H
#include <memory>

#include "block.h"
#include "math/block_pos.h"
#include "math/chunk_pos.h"

#include <boost/multi_array.hpp>

using chunk_data =
boost::multi_array<std::unique_ptr<block>, 3>;

class chunk {
	chunk_pos position;

	chunk_data data{boost::extents[chunk_pos::max_xz][chunk_pos::max_y][chunk_pos::max_xz]};

protected:
	chunk_data&
	get_data() { return data; }

public:
	explicit
	chunk(
		chunk_pos pos);

	std::unique_ptr<block>&
	operator[](
		const block_pos& block_pos) {
		const auto pos = block_pos.get_in_chunk_pos();
		return data[pos.x][pos.y][pos.z];
	}

	void
	serialize(
		std::ostream& os) const;
	void
	deserialize(
		std::istream& is);
};

#endif //CHUNK_H
