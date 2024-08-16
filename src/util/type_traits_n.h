//
// Created by n08i40k on 16.08.2024.
//

#ifndef TYPE_TRAITS_N_H
#define TYPE_TRAITS_N_H
#include <type_traits>

namespace stdn {
// stdn::has_resize[n]
template <typename, typename = std::void_t<> >
struct has_resize : std::false_type {};

template <typename T>
struct has_resize<T, std::void_t<decltype(std::declval<T>().resize(std::declval<T>().size()))> > : std::true_type {};

template <typename T>
inline constexpr bool has_resize_v = has_resize<T>::value;

// stdn::has_size[n]
template <typename, typename = std::void_t<> >
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())> > : std::true_type {};

template <typename T>
inline constexpr bool has_size_v = has_size<T>::value;
}

#endif //TYPE_TRAITS_N_H
