#include "encstrset.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#ifdef NDEBUG
    static const bool debug = true;
#else
    static const bool debug = false;
#endif

#define DIAGNOSTIC_PRINT1(info) do { \
    if (!debug) { std::cerr << info << ": "; } \
    } while(0)

#define DIAGNOSTIC_PRINT2(info) do { \
    if (!debug) { std::cerr << info << std::endl;} \
    } while(0)


namespace {
    // Tu zmienne globalne i funkcje pomocnicze
    using std::unordered_map;
    using std::unordered_set;
    using std::stringstream;
    using std::string;

    unsigned long cur_id = 0;

    string info;
    stringstream sstream;

    unordered_map<unsigned long, unordered_set<string>> map_of_sets;

}

unsigned long jnp1::encstrset_new() {
    DIAGNOSTIC_PRINT1("encstrset_new");

    unordered_set<string> new_set;
    map_of_sets[cur_id] = new_set;

    sstream << "set #" << cur_id << " created";
    info = sstream.str();
    DIAGNOSTIC_PRINT2(info);

    return cur_id++;
}

void jnp1::encstrset_delete(unsigned long id) {
    //
}

size_t jnp1::encstrset_size(unsigned long id) {
    //
}

bool jnp1::encstrset_insert(unsigned long id, const char *value,
                            const char *key) {
    //
}

bool jnp1::encstrset_remove(unsigned long id, const char *value,
                            const char *key) {
    //
}

bool jnp1::encstrset_test(unsigned long id, const char *value,
                          const char *key) {
    //
}

void jnp1::encstrset_clear(unsigned long id) {
    //
}

void jnp1::encstrset_copy(unsigned long src_id, unsigned long dst_id) {
    //
}

int main() {
    jnp1::encstrset_new();


    return 0;
}
