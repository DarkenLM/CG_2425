#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief A generic key-value map wrapper.
 *
 * This class wraps around std::unordered_map to provide simplified
 * key-value storage operations with convenience methods.
 *
 * @tparam V The value type.
 * @tparam K The key type (default is std::string).
 */
template <typename V, typename K = std::string>
class Map {
   public:
    /**
     * @brief Default constructor.
     *
     * Initializes an empty map.
     */
    Map();

    /**
     * @brief Constructs a map from an existing unordered_map.
     *
     * @param map The unordered_map to use as the initial data.
     */
    Map(std::unordered_map<K, V> map);

    /**
     * @brief Checks if the map is empty.
     *
     * @return true if the map has no elements, false otherwise.
     */
    bool isEmpty();

    /**
     * @brief Returns the number of key-value pairs in the map.
     *
     * @return The size of the map.
     */
    int size() const;

    /**
     * @brief Checks whether the specified key exists in the map.
     *
     * @param key The key to look up.
     * @return true if the key is present, false otherwise.
     */
    bool hasKey(K key) const;

    /**
     * @brief Gets the value associated with the given key.
     *
     * @param key The key to retrieve the value for.
     * @return An optional containing the value if found, or std::nullopt.
     */
    std::optional<V> get(K key) const;

    /**
     * @brief Retrieves a list of all keys in the map.
     *
     * @return A vector containing all keys.
     */
    std::vector<K> keys() const;

    /**
     * @brief Retrieves a list of all values in the map.
     *
     * @return A vector containing all values.
     */
    std::vector<V> values() const;

    /**
     * @brief Adds a key-value pair to the map.
     *
     * @param key The key to insert.
     * @param value The value to insert.
     * @return true if insertion was successful, false otherwise.
     */
    bool add(K key, V value);

    /**
     * @brief Sets the value for a key (overwrites if key exists).
     *
     * @param key The key to set the value for.
     * @param value The new value.
     */
    void set(K key, V value);

    /**
     * @brief Removes a key-value pair from the map.
     *
     * @param key The key to remove.
     * @return true if the key was found and removed, false otherwise.
     */
    bool remove(K key);

   private:
    std::unordered_map<K, V> _map;  ///< Internal map storage
};

#include <common/util/maputil.tpp>