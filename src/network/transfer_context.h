//
// Created by n08i40k on 14.08.2024.
//

#ifndef TRANSFER_CONTEXT_H
#define TRANSFER_CONTEXT_H
#include <cstdint>
#include <memory>
#include <vector>

class transfer_context {
public:
	using packet_id_t = uint32_t;

private:
	bool                 receive;
	size_t               size{};
	std::vector<uint8_t> data;

	transfer_context();;

	transfer_context(
		packet_id_t            id,
		std::vector<uint8_t>&& data);

public:
	static std::unique_ptr<transfer_context>
	create_receive() { return std::unique_ptr<transfer_context>{new transfer_context()}; };

	static std::unique_ptr<transfer_context>
	create_send(
		const packet_id_t      id,
		std::vector<uint8_t>&& data) {
		return std::unique_ptr<transfer_context>{new transfer_context(id, std::move(data))};
	};

	[[nodiscard]] std::vector<uint8_t>
	get_data_for_send();

	[[nodiscard]] size_t&
	get_size_for_receive();

	[[nodiscard]] std::vector<uint8_t>&
	get_data_for_receive();

	[[nodiscard]] std::pair<packet_id_t, std::vector<uint8_t> >
	take_data();
};

#endif //TRANSFER_CONTEXT_H
