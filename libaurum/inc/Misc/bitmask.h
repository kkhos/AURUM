/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef _BITMASK_H_
#define _BITMASK_H_

#include <type_traits>

namespace Aurum {

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

}

#endif
