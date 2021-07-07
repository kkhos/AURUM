#pragma once

//#include <atspi/atspi.h>
#include <gio/gio.h>

#include <string.h>
#include <memory>

#include "config.h"

/**
 * @brief TBD
 * @since_tizen 5.5
 */
struct GobjDeletor {
    void operator()(gpointer ptr) const
    {
        if (ptr) g_object_unref(ptr);
    }
};

/**
 * @brief TBD
 * @since_tizen 5.5
 */
struct GarrayDeletor {
    void operator()(GArray *ptr) const
    {
        if (ptr) g_array_free(ptr, 1);
    }
};

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
using unique_ptr_gobj = std::unique_ptr<T, GobjDeletor>;

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
using unique_ptr_garray = std::unique_ptr<T, GarrayDeletor>;

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
unique_ptr_gobj<T> make_gobj_unique(T *ptr)
{
    return unique_ptr_gobj<T>(ptr);
}

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
unique_ptr_garray<T> make_garray_unique(T *ptr)
{
    return unique_ptr_garray<T>(ptr);
}

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
unique_ptr_gobj<T> make_gobj_ref_unique(T *ptr)
{
    g_object_ref(ptr);
    return unique_ptr_gobj<T>(ptr);
}

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
std::shared_ptr<T> make_gobj_shared(T *ptr)
{
    return std::shared_ptr<T>(ptr, [](T *ptr){ if(ptr) g_object_unref(ptr); });
}

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
std::shared_ptr<T> make_garray_shared(T *ptr)
{
    return std::shared_ptr<T>(ptr,  [](T *ptr){ if(ptr) g_array_free(ptr, 1); });
}

/**
 * @brief TBD
 * @since_tizen 5.5
 */
template <class T>
std::shared_ptr<T> make_gobj_ref_shared(T *ptr)
{
    g_object_ref(ptr);
    return std::shared_ptr<T>(ptr,  [](T *ptr){ if(ptr) g_object_unref(ptr); });
}

/**
 * @brief TBD
 * @since_tizen 5.5
 */
//char *state_to_char(AtspiStateType state);
