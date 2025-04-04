#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

// /**
//  * Checks if a given key x exists within a map m.
//  * Also creates a reference to the KVP with the name defined by the macro `MAPUTIL_NAME`
//  *
//  * @param m The map to search.
//  * @param k The key to search for.
//  */
// #define HAS_AND_CAPTURE_KEY(m, k) auto MAPUTIL_NAME{ m.find(k) }; MAPUTIL_NAME != std::end( m )

// #define PREPARE_KVP(MAPUTIL_NAME) const auto&[ key_##MAPUTIL_NAME, value_##MAPUTIL_NAME ] { *MAPUTIL_NAME };
// #define PREPARE_KVP_DESTRUCTED() const auto& kvp_##MAPUTIL_NAME { *MAPUTIL_NAME };
// // #define PREPARE_KEY() const auto& key{ MAPUTIL_NAME->first };
// // #define PREPARE_VALUE() const auto& value{ MAPUTIL_NAME->second };

// #define GET_KVP(MAPUTIL_NAME) kvp_##MAPUTIL_NAME
// #define GET_KEY(MAPUTIL_NAME) key_##MAPUTIL_NAME
// #define GET_VALUE(MAPUTIL_NAME) value_##MAPUTIL_NAME

// #define GENERATE_FUNCTION(RET,NAM,...) RET func_##NAM(__VA_ARGS__)

template <typename V, typename K = std::string>
class Map {
   public:
    Map() = default;
    Map(std::unordered_map<K, V> map) {
        this->_map = map;
    }

    bool hasKey(K key) const {
        auto _kvp{this->_map.find(key)};
        return _kvp != std::end(this->_map);
    }

    std::optional<V> get(K key) const {
        auto _kvp = this->_map.find(key);
        if (_kvp == std::end(this->_map)) return std::nullopt;

        // return std::optional<V> _kvp->second;
        const auto& value{_kvp->second};
        return value;
    }

    bool add(K key, V value) {
        // auto res = this->_map.insert_or_assign(std::make_pair(key, value));

        // auto res = this->_map.insert_or_assign(key, value);
        // return res->second;

        // this->_map.insert(std::make_pair(key, value));
        this->_map.emplace(key, value);
        return true;
    }

    // Paulo Functions, improve if necessary
    /**
     * This function should return an array with all values
     */
    std::vector<V> getValues() const {
        std::vector<V> r;
        for (const auto& [key, value] : this->_map) {
            // std::cout << key << std::endl;
            r.push_back(value);
        }
        return r;
    }
    /**
     * This function should return an array with all keys
     */
    std::vector<K> getKeys() const {
        std::vector<K> r;
        for (const auto& [key, value] : this->_map) {
            // std::cout << key << std::endl;
            r.push_back(key);
        }
        return r;
    }
    /**
     * This function should return the amount of values in the map
     */
    int size() const {
        int r = 0;
        for (const auto& [key, value] : this->_map) {
            r++;
        }
        return r;
    }

   private:
    std::unordered_map<K, V> _map;
};