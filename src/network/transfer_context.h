//
// Created by n08i40k on 14.08.2024.
//

#ifndef TRANSFER_CONTEXT_H
#define TRANSFER_CONTEXT_H
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "packet_id.h"

namespace network {
class transfer_context {
	bool        receive;
	size_t      size{};
	std::string data;

	transfer_context();

	transfer_context(
		packet_id_t   id,
		std::string&& data);

public:
	static std::unique_ptr<transfer_context>
	create_receive() { return std::unique_ptr<transfer_context>{new transfer_context()}; };

	static std::unique_ptr<transfer_context>
	create_send(
		const packet_id id,
		std::string&&   data) {
		return std::unique_ptr<transfer_context>{new transfer_context(static_cast<packet_id_t>(id), std::move(data))};
	};

	[[nodiscard]] std::string
	get_data_for_send();

	[[nodiscard]] size_t&
	get_size_for_receive();

	[[nodiscard]] std::string&
	get_data_for_receive();

	[[nodiscard]] std::pair<packet_id, std::string>
	take_data();
};
}

#endif //TRANSFER_CONTEXT_H
