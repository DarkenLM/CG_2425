#pragma once

// This shit works... IntelliSense is making me stupid and im done with that
// Its fucking staying like that
// P.S: Sometimes red lines appear, sometimes they don't. Thanks for proving mi point (╯°□°)╯︵ ┻━┻
template <typename V, typename K>
Map<V, K>::Map() = default;

template <typename V, typename K>
Map<V, K>::Map(std::unordered_map<K, V> map) {
    this->_map = map;
}

template <typename V, typename K>
bool Map<V, K>::isEmpty() {
    return this->_map.empty();
}

template <typename V, typename K>
int Map<V, K>::size() const {
    int r = 0;
    for (const auto& [key, value] : this->_map) {
        r++;
    }
    return r;
}

template <typename V, typename K>
bool Map<V, K>::hasKey(K key) const {
    auto _kvp{this->_map.find(key)};
    return _kvp != std::end(this->_map);
}

template <typename V, typename K>
std::optional<V> Map<V, K>::get(K key) const {
    auto _kvp = this->_map.find(key);
    if (_kvp == std::end(this->_map)) return std::nullopt;

    const auto& value{_kvp->second};
    return value;
}

template <typename V, typename K>
std::vector<K> Map<V, K>::keys() const {
    std::vector<K> keys;
    for (auto it = this->_map.begin(); it != this->_map.end(); ++it) {
        keys.push_back(it->first);
    }

    return keys;
}

template <typename V, typename K>
std::vector<V> Map<V, K>::values() const {
    std::vector<V> values;
    for (auto it = this->_map.begin(); it != this->_map.end(); ++it) {
        values.push_back(it->second);
    }

    return values;
}

template <typename V, typename K>
bool Map<V, K>::add(K key, V value) {
    this->_map.emplace(key, value);
    return true;
}

template <typename V, typename K>
void Map<V, K>::set(K key, V value) {
    this->_map[key] = value;
}

template <typename V, typename K>
bool Map<V, K>::remove(K key) {
    auto it = this->_map.find(key);
    if (it != this->_map.end()) {
        this->_map.erase(it);
        return true;
    }

    return false;
};