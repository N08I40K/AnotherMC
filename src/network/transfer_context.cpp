//
// Created by n08i40k on 14.08.2024.
//

#include "transfer_context.h"

#include <ostream>

#include "util/bstream.h"

namespace network {
transfer_context::transfer_context(): receive{true} {}

transfer_context::transfer_context(
	const packet_id_t id,
	std::string&&     data)
	: receive{false} {
	stdn::bwstream write_stream;

	write_stream << data.size() + sizeof(packet_id_t);
	write_stream << id;
	write_stream << data;

	this->data = std::move(write_stream.take_result<std::string>());
}

std::string
transfer_context::get_data_for_send() {
	if (this->receive)
		throw std::logic_error("Этот transfer_context должен использоваться только для получения данных!");
	return std::move(this->data);
}

size_t&
transfer_context::get_size_for_receive() {
	if (!this->receive)
		throw std::logic_error("Этот transfer_context должен использоваться только для отправки данных!");
	return this->size;
}

std::string&
transfer_context::get_data_for_receive() {
	if (!this->receive)
		throw std::logic_error("Этот transfer_context должен использоваться только для отправки данных!");

	if (data.size() != size)
		data.resize(size);
	return data;
}

std::pair<packet_id, std::string>
transfer_context::take_data() {
	if (!this->receive)
		throw std::logic_error("Этот transfer_context должен использоваться только для отправки данных!");

	stdn::brstream read_stream{data};

	packet_id_t id;
	std::string packet_data;

	read_stream >> id >> packet_data;
	this->data.clear();

	return {static_cast<packet_id>(id), std::move(packet_data)};
}
}
