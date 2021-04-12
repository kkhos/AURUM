#pragma once
#include <type_traits>

template<typename E>
struct enable_bitmask_operators{
    static const bool enable=false;
};

#define enableEnumClassBitfield(E) template<> \
        struct enable_bitmask_operators<E>{   \
            static const bool enable=true;    \
        }

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::enable,E>
operator|(E lhs,E rhs){
    typedef typename std::underlying_type_t<E> underlying;
    return static_cast<E>(
        static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::enable,E>
operator&(E lhs,E rhs){
    typedef typename std::underlying_type_t<E> underlying;
    return static_cast<E>(
        static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::enable,E>
operator^(E lhs,E rhs){
    typedef typename std::underlying_type_t<E> underlying;
    return static_cast<E>(
        static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::enable,E>
operator~(E lhs){
    typedef typename std::underlying_type_t<E> underlying;
    return static_cast<E>(
        ~static_cast<underlying>(lhs));
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::enable,E&>
operator|=(E& lhs,E rhs){
    typedef typename std::underlying_type_t<E> underlying;
    lhs=static_cast<E>(
        static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
    return lhs;
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::enable,E&>
operator&=(E& lhs,E rhs){
    typedef typename std::underlying_type_t<E> underlying;
    lhs=static_cast<E>(
        static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
    return lhs;
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::enable,E&>
operator^=(E& lhs,E rhs){
    typedef typename std::underlying_type_t<E> underlying;
    lhs=static_cast<E>(
        static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
    return lhs;
}

