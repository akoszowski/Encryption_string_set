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

//#define DIAGNOSTIC_PRINT1(info) do { \
//    if (!debug) { std::cerr << info << ": "; } \
//    } while(0)
//
//#define DIAGNOSTIC_PRINT2(info) do { \
//    if (!debug) { std::cerr << info << std::endl;} \
//    } while(0)

#define DIAGNOSTIC_PRINT(info) do { \
    if (!debug) { std::cerr << info << std::endl;} \
    } while(0)

#define DIAGNOSTIC_PRINT_SET_ERR(function_name, set_number) do { \
    if (!debug) { std::cerr << function_name << ": set" << set_number << " does not exist" << std::endl;} \
    } while(0)

#define DIAGNOSTIC_PRINT_NULL_VALUE(function_name) do { \
    if (!debug) { std::cerr << function_name << ": invalid value (NULL)" << std::endl;} \
    } while(0)


namespace {
    // Tu zmienne globalne i funkcje pomocnicze
    using std::unordered_map;
    using std::unordered_set;
    using std::stringstream;
    using std::string;
    using std::cout;
    unsigned long cur_id = 0;
    using std::to_string;

    string info;
    stringstream sstream;

    unordered_map<unsigned long, unordered_set<string>> map_of_sets;


    void diag_print(const string& func_name, unsigned long id, string & value, string & key, bool value_is_null, bool key_is_null) {
        if (value_is_null && key_is_null) {
            DIAGNOSTIC_PRINT("encstrset_insert(" + to_string(id) + ", " + "NULL" + ", " + "NULL)");
        }
        else if (value_is_null) {
            DIAGNOSTIC_PRINT("encstrset_insert(" + to_string(id) + ", NULL" + ", \"" + key + "\")");
            // nie wiem czy tutaj ten print czy np. po set doesn't exist ma być wypisywane bo zamiast specyfikacji mamy przykłady które takich przypadków nie wyjaśniają
        }
        else if (key_is_null) {
            DIAGNOSTIC_PRINT("encstrset_insert(" + to_string(id) + ", \"" + value + "\", NULL)");
        }
    }

    string to_hex(string & value) {
        stringstream stream;
        for (char c : value) {
            stream << std::hex << std::uppercase << (unsigned int)c << " ";
        }
        cout << stream.str() << std::endl;
        return stream.str();
    }

    string encrypt(string & value, string & key, bool  key_is_null) {
        string encrypted_value = string(value);
        if (key_is_null) return encrypted_value;
        for (int i = 0; i < value.length(); i++) {
            encrypted_value[i] = value[i] ^ key[i % key.length()];
        }
        return encrypted_value;
    }

}

namespace jnp1 {
    unsigned long encstrset_new() {
        DIAGNOSTIC_PRINT("encstrset_new()");

        unordered_set<string> new_set;
        map_of_sets[cur_id] = new_set;

        sstream << "set #" << cur_id << " created";
        info = sstream.str();
        DIAGNOSTIC_PRINT("encstrset_new: " + info);

        return cur_id++;
    }

    void encstrset_delete(unsigned long id) {
        DIAGNOSTIC_PRINT("encstrset_delete(" + to_string(id) + ")");
        auto it = map_of_sets.find(id);
        if (it != map_of_sets.end()) {
            map_of_sets.erase(it);
        }
        else {
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_delete", to_string(id));
        }
    }

    size_t encstrset_size(unsigned long id) {
        DIAGNOSTIC_PRINT("encstrset_size(" + to_string(id) + ")");
        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_size", to_string(id));
            return 0;
        }
        return it->second.size();
    }

    bool encstrset_insert(unsigned long id, const char *value,
                                const char *key) {
        string v; string k;
        bool value_is_null = false;
        bool key_is_null = false;
        if (value != NULL) v = string(value);
        else value_is_null = true;
        if (key != NULL) k = string(key);
        else key_is_null = true;
        
        diag_print("encstrset_insert", id, v, k, value_is_null, key_is_null);

        if (value_is_null) {
            DIAGNOSTIC_PRINT_NULL_VALUE("encstrset_insert");
            return false;
        }

        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_insert", to_string(id));
            return false;
        }

        string cypher = encrypt(v, k, key_is_null);
        unordered_set<string> & s = it->second;
        auto elem = s.find(cypher);
        if (elem == s.end()) {
            s.insert(cypher);
            DIAGNOSTIC_PRINT("encstrset_insert: set # " + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" inserted");
            return true;
        }
        else {
            // diagnostic msg jakis
            DIAGNOSTIC_PRINT("encstrset_insert: set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" was already present");
        }
        return false;
    }

    bool encstrset_remove(unsigned long id, const char *value,
                                const char *key) {
        string v; string k;
        bool value_is_null = false;
        bool key_is_null = false;
        if (value != NULL) v = string(value);
        else value_is_null = true;
        if (key != NULL) k = string(key);
        else key_is_null = true;

        diag_print("encstrset_remove", id, v, k, value_is_null, key_is_null);
        if (value_is_null) {
            DIAGNOSTIC_PRINT_NULL_VALUE("encstrset_remove");
            return false;
        }
        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_remove", to_string(id));
            return false;
        }

        string cypher = encrypt(v, k, key_is_null);
        unordered_set<string> & s = it->second;
        auto elem = s.find(cypher);
        if (elem == s.end()) {
            DIAGNOSTIC_PRINT("encstrset_remove: set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" was not present");
            return false;
        }
        s.erase(elem);
        DIAGNOSTIC_PRINT("encstrset_remove: set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" removed");
        return true;

    }

    bool encstrset_test(unsigned long id, const char *value,
                              const char *key) {
        string v; string k;
        bool value_is_null = false;
        bool key_is_null = false;
        if (value != NULL) v = string(value);
        else value_is_null = true;
        if (key != NULL) k = string(key);
        else key_is_null = true;

        diag_print("encstrset_test", id, v, k, value_is_null, key_is_null);

        if (value_is_null) {
            DIAGNOSTIC_PRINT_NULL_VALUE("encstrset_test");
            return false;
        }

        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_test", to_string(id));
            return false;
        }

        string cypher = encrypt(v, k, key_is_null);
        unordered_set<string> & s = it->second;
        auto it2 = s.find(cypher);
        if (it2 == s.end()) {
            return false;
        }
        return true;
    }

    void encstrset_clear(unsigned long id) {
        DIAGNOSTIC_PRINT("encstrset_clear(" + to_string(id) + ")");
        auto it = map_of_sets.find(id);
        if (it == map_of_sets.end()) {
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_clear", to_string(id));
        }
        else {
            unordered_set<string> & s = it->second;
            for (auto it2 = s.begin(); it2 != s.end(); it++) {
                s.erase(it2);
            }
        }
        DIAGNOSTIC_PRINT("encstrset_clear: set #" + to_string(id) + " cleared");
    }

    void encstrset_copy(unsigned long src_id, unsigned long dst_id) {
        auto find_src = map_of_sets.find(src_id);
        auto find_dst = map_of_sets.find(dst_id);
        if (find_src == map_of_sets.end()) {
            //diagnostic message does not exist
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_copy", to_string(src_id));
        }
        else if (find_dst == map_of_sets.end()) {
            //diagnostic message does not exist
            DIAGNOSTIC_PRINT_SET_ERR("encstrset_copy", to_string(dst_id));
        }
        else {
            unordered_set<string> src_set = find_src->second;
            unordered_set<string> dst_set = find_dst->second;
            for (auto el : src_set) {
                if (dst_set.find(el) != dst_set.end()) {
                    DIAGNOSTIC_PRINT("encstrset_copy: copied cypher \"" + to_hex(el) +
                                       "\" was already present in set #" + to_string(dst_id));
                }
                else {
                    dst_set.insert(el);
                    DIAGNOSTIC_PRINT("encstrset_copy: cypher \"" + to_hex(el) + "\" copied from set #" +
                                       to_string(src_id) + " to set #" + to_string(dst_id));
                }
            }
        }
    }
}

int main() {
    jnp1::encstrset_new();

    const char* value = "foo";
    const char* key = "123";
    string value_copy = string(value);
    string key_copy = string(key);
    value_copy = encrypt(value_copy, key_copy, false);
    cout << to_hex(value_copy);

    return 0;
}
