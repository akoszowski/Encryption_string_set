#ifndef ENCSTRSET_H
#define ENCSTRSET_H

#ifndef __cplusplus
    #include <stdlib.h>
#endif

#ifdef __cplusplus
    #include <cstdlib>

    extern "C" {

    namespace jnp1 {
#endif

// Creates new set. Returns its [id].
        unsigned long encstrset_new();

// If set with [id] exists, it is removed. Nothing happens otherwise.
        void encstrset_delete(unsigned long id);

// If set with [id] exists, returns its number of elements, 0 otherwise.
        size_t encstrset_size(unsigned long id);

// If set with [id] exists and element [value] after encoding it with
// [cipher] is not a part of the set, then the element is added [true];
// otherwise nothing happens [false].
        bool
        encstrset_insert(unsigned long id, const char *value, const char *key);

// If set with [id] exists and element [value] after encoding it with
// [cipher] is a part of the set, then the element is removed [true];
// otherwise nothing happens [false].
        bool
        encstrset_remove(unsigned long id, const char *value, const char *key);

// If set with [id] exists and element [value] after encoding it with
// [cipher] is a part of the set, then returns [true];
// otherwise returns [false].
        bool
        encstrset_test(unsigned long id, const char *value, const char *key);

// If set with [id] exists, removes its all elements;
// otherwise nothing happens.
        void encstrset_clear(unsigned long id);

// If both set with [src_id] and [dst_id] exist, content of the first one
// is copied into the second one; otherwise nothing happens.
        void encstrset_copy(unsigned long src_id, unsigned long dst_id);

#ifdef __cplusplus
    }

    }
#endif

#endif //ENCSTRSET_H
