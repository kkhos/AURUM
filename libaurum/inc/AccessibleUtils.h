#ifndef ACCESSIBLE_UTILS_H
#define ACCESSIBLE_UTILS_H

#include <atspi/atspi.h>
#include <gio/gio.h>

#include <memory>
#include "config.h"

struct GobjDeletor {
    void operator() (gpointer ptr) const { if (ptr) g_object_unref(ptr); }
};

struct GarrayDeletor {
    void operator() (GArray *ptr) const { if (ptr) g_array_free(ptr, 1); }
};

template<class T> using unique_ptr_gobj = std::unique_ptr<T, GobjDeletor>;
template<class T> using unique_ptr_garray = std::unique_ptr<T, GarrayDeletor>;

template<class T>
unique_ptr_gobj<T> make_gobj_unique(T *ptr)
{
    return unique_ptr_gobj<T>(ptr);
}

template<class T>
unique_ptr_garray<T> make_garray_unique(T *ptr)
{
    return unique_ptr_garray<T>(ptr);
}

template<class T>
unique_ptr_gobj<T> make_gobj_ref_unique(T *ptr)
{
    g_object_ref(ptr);
    return unique_ptr_gobj<T>(ptr);
}

char *state_to_char(AtspiStateType state);

#endif