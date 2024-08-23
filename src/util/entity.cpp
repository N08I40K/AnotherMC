//
// Created by n08i40k on 23.08.2024.
//

#include "entity.h"

#include "world/entity/entity.h"
#include <entity_data.pb.h>

#include "bstream.h"

namespace utils::entity {
void
parse_data(
	::entity*          entity,
	const entity_data& data) {
	stdn::brstream stream{data.data()};
	entity->parse_data(stream);
}

entity_data
serialize_data(
	::entity* entity) {
	stdn::bwstream stream;
	entity->serialize_data(stream);

	entity_data entity_data;
	entity_data.set_data(stream.take_result<std::string>());

	return entity_data;
}
}
