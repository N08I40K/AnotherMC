//
// Created by n08i40k on 13.08.2024.
//

#ifndef CLASS_OPERATIONS_H
#define CLASS_OPERATIONS_H
#include <type_traits>

// #include <typeinfo>

namespace util {
// class type_id_expected {
// public:
// 	virtual ~type_id_expected() = default;
//
// 	[[nodiscard]] virtual size_t&
// 	get_type_hash() const = 0;
// };
//
// #define DEFINE_TYPE_ID_FUNC [[nodiscard]] size_t\
// 	get_type_id() const override { return typeid(decltype(*this)).hash_code(); }
//
// template <class T>
// class type_id_definition : public type_id_expected {
// public:
// 	DEFINE_TYPE_ID_FUNC
// };
//

template <class BaseT>
class cloneable {
public:
	virtual ~cloneable() = default;

	[[nodiscard]] virtual BaseT*
	clone() const = 0;
};

template <class T>
concept Cloneable = requires(
	T) {
			std::is_base_of_v<cloneable<T>, T>;
		};
}

#endif //CLASS_OPERATIONS_H
