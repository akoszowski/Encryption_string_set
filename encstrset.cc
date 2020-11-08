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
    using std::cout;
    unsigned long cur_id = 0;

    string info;
    stringstream sstream;

    unordered_map<unsigned long, unordered_set<string>> map_of_sets;

    string to_hex(string & value) {
        stringstream stream;
        for (char c : value) {
            stream << std::hex << std::uppercase << (unsigned int)c << " ";
        }
        cout << stream.str() << std::endl;
        return stream.str();
    }

    string encrypt(string & value, string & key) {
        string encrypted_value = string(value);
        for (int i = 0; i < value.length(); i++) {
            encrypted_value[i] = value[i] ^ key[i % key.length()];
        }
        return encrypted_value;
    }

}

namespace jnp1 {
    unsigned long encstrset_new() {
        DIAGNOSTIC_PRINT1("encstrset_new");

        unordered_set<string> new_set;
        map_of_sets[cur_id] = new_set;

        sstream << "set #" << cur_id << " created";
        info = sstream.str();
        DIAGNOSTIC_PRINT2(info);

        return cur_id++;
    }

    void encstrset_delete(unsigned long id) {
        auto it = map_of_sets.find(id);
        if (it != map_of_sets.end()) {
            map_of_sets.erase(it);
        }
    }

    size_t encstrset_size(unsigned long id) {
        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            return 0;
        }
        return it->second.size();
    }

    bool encstrset_insert(unsigned long id, const char *value,
                                const char *key) {

        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            return false;
        }
        string v = string(value);
        string k = string(key);
        string cypher = encrypt(v, k);
        unordered_set<string> & s = it->second;
        auto it2 = s.find(cypher);
        if (it2 == s.end()) {
            s.insert(cypher);
            return true;
        }
        return false;
    }

    bool encstrset_remove(unsigned long id, const char *value,
                                const char *key) {
        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            return false;
        }
        string v = string(value);
        string k = string(key);
        string cypher = encrypt(v, k);
        unordered_set<string> & s = it->second;
        auto it2 = s.find(cypher);
        if (it2 == s.end()) {
            return false;
        }
        s.erase(it2);
        return true;

    }

    bool encstrset_test(unsigned long id, const char *value,
                              const char *key) {
        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            return false;
        }
        string v = string(value);
        string k = string(key);
        string cypher = encrypt(v, k);
        unordered_set<string> & s = it->second;
        auto it2 = s.find(cypher);
        if (it2 == s.end()) {
            return false;
        }
        return true;
    }

    void encstrset_clear(unsigned long id) {
        auto it = map_of_sets.find(id);
        if (it != map_of_sets.end()) {
            unordered_set<string> & s = it->second;
            for (auto it2 = s.begin(); it2 != s.end(); it++) {
                s.erase(it2);
            }
        }
    }

    void encstrset_copy(unsigned long src_id, unsigned long dst_id) {
        auto find_src = map_of_sets.find(src_id);
        auto find_dst = map_of_sets.find(dst_id);
        if (find_src == map_of_sets.end()) {
            //diagnostic message does not exist
            return;
        }
        if (find_src == map_of_sets.end()) {
            //diagnostic message does not exist
            return;
        }
        else {

        }
    }
}

int main() {
    jnp1::encstrset_new();

    const char* value = "foo";
    const char* key = "123";
    string value_copy = string(value);
    string key_copy = string(key);
    value_copy = encrypt(value_copy, key_copy);
    cout << to_hex(value_copy);

    return 0;
}
