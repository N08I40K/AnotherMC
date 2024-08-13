//
// Created by n08i40k on 13.08.2024.
//

#ifndef REGISTRY_H
#define REGISTRY_H
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "util/class_operations.h"

template <util::Cloneable T>
class registry {
	std::map<std::string, std::unique_ptr<T*> > by_name;
	std::vector<T*>                             by_idx;

public:
	registry();

	size_t
	add(
		std::string        name,
		std::unique_ptr<T> base) {
		if (by_name.contains(name))
			throw std::runtime_error("registry::add: duplicate name");

		by_name[name] = std::move(base);
		by_idx.emplace_back(by_name[name].get());

		return by_idx.size() - 1;
	}

	[[nodiscard]] T*
	operator[](
		const std::string& name) const {
		auto it = by_name.find(name);
		if (it == by_name.end())
			return nullptr;

		return it->second.get();
	}

	[[nodiscard]] T*
	operator[](
		size_t idx) const {
		if (idx >= by_idx.size())
			return nullptr;
		return by_idx[idx];
	}
};

#endif //REGISTRY_H
