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


// TODO Linesize 80
#define DIAGNOSTIC_PRINT(info) do { \
    if (!debug) { std::cerr << info << std::endl;} \
    } while(0)

#define DIAGNOSTIC_PRINT_SET_ERR(func_name, set_number) do { \
    if (!debug) { std::cerr << func_name << ": set #" << set_number << " does not exist" << std::endl;} \
    } while(0)

#define DIAGNOSTIC_PRINT_NULL_VALUE(func_name) do { \
    if (!debug) { std::cerr << func_name << ": invalid value (NULL)" << std::endl;} \
    } while(0)


namespace {
    using std::unordered_map;
    using std::unordered_set;
    using std::string;
    using std::to_string;
    using std::stringstream;

    unsigned long& cur_id() {
        static unsigned long cur_id = 0;

        return cur_id;
    }

    //Static initialization
    auto& map_of_sets() {
        static unordered_map<unsigned long, unordered_set<string>> map_of_sets;

        return map_of_sets;
    }

   void check_params(const string &func_name, unsigned long id, string &value, string &key, bool value_is_null, bool key_is_null) {
        if (value_is_null && key_is_null) {
            DIAGNOSTIC_PRINT(func_name + "(" + to_string(id) + ", " + "NULL" + ", " + "NULL)");
        }
        else if (value_is_null) {
            DIAGNOSTIC_PRINT(func_name + "(" + to_string(id) + ", NULL" + ", \"" + key + "\")");
        }
        else if (key_is_null) {
            DIAGNOSTIC_PRINT(func_name + "(" + to_string(id) + ", \"" + value + "\", NULL)");
        }
        else {
            DIAGNOSTIC_PRINT(func_name + "(" + to_string(id) + ", \"" + value + "\", \"" + key + "\")");
        }
    }

    string to_hex(string &value) {
        size_t  i;
        stringstream stream;

        for (i = 0; i < value.size()-1; ++i) {
            stream << std::hex << std::uppercase << (unsigned int)value[i] << " ";
        }
        stream << std::hex << std::uppercase << (unsigned int)value[i];

        return stream.str();
    }

    string encrypt(string &value, string &key, bool key_is_null) {
        string encrypted_value = string(value);

        if (key_is_null) return encrypted_value;

        for (size_t i = 0; i < value.length(); i++) {
            encrypted_value[i] = value[i] ^ key[i % key.length()];
        }

        return encrypted_value;
    }
}

namespace jnp1 {

    unsigned long encstrset_new() {
        string func_name = "encstrset_new";
        DIAGNOSTIC_PRINT(func_name + "()");

        unordered_set<string> new_set;
        map_of_sets()[cur_id()] = new_set;
        DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(cur_id()) + " created");

        return cur_id()++;
    }

    void encstrset_delete(unsigned long id) {
        string func_name = "encstrset_delete";
        DIAGNOSTIC_PRINT(func_name + "(" + to_string(id) + ")");

        auto it = map_of_sets().find(id);
        if (it != map_of_sets().end()) {
            map_of_sets().erase(it);
            DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + " deleted");
        }
        else {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(id));
        }
    }

    size_t encstrset_size(unsigned long id) {
        string func_name = "encstrset_size";
        DIAGNOSTIC_PRINT(func_name + "(" + to_string(id) + ")");

        auto it = map_of_sets().find(id);
        if (it == map_of_sets().end()) {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(id));
            return 0;
        }

        size_t size = it->second.size();
        DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + " contains " + to_string(size) + " element(s)");

        return size;
    }

    bool encstrset_insert(unsigned long id, const char *value, const char *key) {
        string v, k, func_name = "encstrset_insert";
        bool value_is_null = false, key_is_null = false;

        if (value != nullptr) v = string(value);
        else value_is_null = true;
        if (key != nullptr && key[0] != '\0') k = string(key);
        else key_is_null = true;

        check_params(func_name, id, v, k, value_is_null, key_is_null);

        if (value_is_null) {
            DIAGNOSTIC_PRINT_NULL_VALUE(func_name);
            return false;
        }

        auto it = map_of_sets().find(id);
        if (it == map_of_sets().end()) {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(id));
            return false;
        }

        string cypher = encrypt(v, k, key_is_null);
        unordered_set<string> &s = it->second;

        auto elem = s.find(cypher);
        if (elem == s.end()) {
            s.insert(cypher);
            DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" inserted");
            return true;
        }
        else {
            DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" was already present");
        }

        return false;
    }

    bool encstrset_remove(unsigned long id, const char *value, const char *key) {
        string v, k, func_name = "encstrset_remove";
        bool value_is_null = false, key_is_null = false;

        if (value != NULL) v = string(value);
        else value_is_null = true;
        if (key != NULL && key[0] != '\0') k = string(key);
        else key_is_null = true;

        check_params(func_name, id, v, k, value_is_null, key_is_null);

        if (value_is_null) {
            DIAGNOSTIC_PRINT_NULL_VALUE(func_name);
            return false;
        }

        auto it = map_of_sets().find(id);
        if (it == map_of_sets().end()) {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(id));
            return false;
        }

        string cypher = encrypt(v, k, key_is_null);
        unordered_set<string> &s = it->second;

        auto elem = s.find(cypher);
        if (elem == s.end()) {
            DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" was not present");
            return false;
        }
        s.erase(elem);
        DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" removed");

        return true;
    }

    bool encstrset_test(unsigned long id, const char *value, const char *key) {
        string v, k, func_name = "encstrset_test";
        bool value_is_null = false, key_is_null = false;

        if (value != NULL) v = string(value);
        else value_is_null = true;
        if (key != NULL && key[0] != '\0') k = string(key);
        else key_is_null = true;

        check_params(func_name, id, v, k, value_is_null, key_is_null);

        if (value_is_null) {
            DIAGNOSTIC_PRINT_NULL_VALUE(func_name);
            return false;
        }

        auto it = map_of_sets().find(id);
        if (it == map_of_sets().end()) {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(id));
            return false;
        }

        string cypher = encrypt(v, k, key_is_null);
        unordered_set<string> &s = it->second;

        auto elem = s.find(cypher);
        if (elem == s.end()) {
            DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" is not present");
            return false;
        }
        DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + ", cypher \"" + to_hex(cypher) + "\" is present");

        return true;
    }

    void encstrset_clear(unsigned long id) {
        string func_name = "encstrset_clear";
        DIAGNOSTIC_PRINT(func_name + "(" + to_string(id) + ")");

        auto it = map_of_sets().find(id);
        if (it == map_of_sets().end()) {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(id));
        }
        else {
            unordered_set<string> &s = it->second;
            s.clear();
            DIAGNOSTIC_PRINT(func_name + ": set #" + to_string(id) + " cleared");
        }
    }

    void encstrset_copy(unsigned long src_id, unsigned long dst_id) {
        string func_name = "encstrset_copy";
        DIAGNOSTIC_PRINT(func_name + "(" + to_string(src_id) + ", " + to_string(dst_id) + ")");

        auto find_src = map_of_sets().find(src_id);
        auto find_dst = map_of_sets().find(dst_id);
        if (find_src == map_of_sets().end()) {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(src_id));
        } else if (find_dst == map_of_sets().end()) {
            DIAGNOSTIC_PRINT_SET_ERR(func_name, to_string(dst_id));
        } else {
            unordered_set<string> &src_set = find_src->second;
            unordered_set<string> &dst_set = find_dst->second;
            for (auto elem : src_set) {
                if (dst_set.find(elem) != dst_set.end()) {
                    DIAGNOSTIC_PRINT(
                            func_name + ": copied cypher \"" + to_hex(elem) +
                            "\" was already present in set #" + to_string(dst_id));
                } else {
                    dst_set.insert(elem);
                    DIAGNOSTIC_PRINT(func_name + ": cypher \"" + to_hex(elem) +
                                     "\" copied from set #" +
                                     to_string(src_id) + " to set #" +
                                     to_string(dst_id));
                }
            }
        }
    }
}