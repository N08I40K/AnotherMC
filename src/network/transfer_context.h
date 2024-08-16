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
	struct data {
		uint32_t             id;
		std::vector<uint8_t> bytes;
	};

private:
	size_t               size{};
	std::vector<uint8_t> bytes;

	transfer_context() = default;

	transfer_context(
		const uint32_t         id,
		std::vector<uint8_t>&& data);;

public:
	static std::unique_ptr<transfer_context>
	create_receive() { return std::unique_ptr<transfer_context>{new transfer_context()}; };

	static std::unique_ptr<transfer_context>
	create_send(
		const uint32_t         id,
		std::vector<uint8_t>&& data) {
		return std::unique_ptr<transfer_context>{new transfer_context(id, std::move(data))};
	};
};

#endif //TRANSFER_CONTEXT_H
