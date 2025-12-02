#ifndef HHASHMAP_TPP
#define HHASHMAP_TPP

#include "Hhashmap.h"

namespace shed_std {

// -------------------------------------------------------------------
// 主模板类（链表版本）实现
// -------------------------------------------------------------------

// 私有成员函数
template <typename K, typename V, typename Hash, typename Enable>
void Hhashmap<K, V, Hash, Enable>::_auto_expand() {
    int current_capacity = _data.size();

    // 初始化容量（防止空桶数组）
    if (current_capacity == 0) {
        rehash(DEFAULT_INITIAL_CAPACITY);
        return;
    }

    // 负载因子检查
    if (static_cast<double>(_size) > static_cast<double>(current_capacity) * LOAD_FACTOR) {
        int new_capacity = current_capacity * 2;
        if (new_capacity > 0) {  // 防止溢出
            rehash(new_capacity);
        }
    }
}

template <typename K, typename V, typename Hash, typename Enable>
void Hhashmap<K, V, Hash, Enable>::rehash(int new_capacity) {
    if (new_capacity <= _data.size()) {
        return;  // 新容量必须大于当前容量
    }

    Aarray<Llist<pair<K, V>>> new_buckets(new_capacity);

    // 迁移旧元素到新桶
    int old_capacity = _data.size();
    for (int i = 0; i < old_capacity; ++i) {
        Llist<pair<K, V>>& old_list = _data[i];
        for (typename Llist<pair<K, V>>::Llist_iterator it = old_list.begin(); it != old_list.end(); ++it) {
            pair<K, V> p = *it;
            int hash_val = static_cast<int>(_hash(p.first));
            int new_index = hash_val & (new_capacity - 1);  // 利用2的幂特性取模
            new_buckets[new_index].add_at_front(p);
        }
    }

    _data = new_buckets;  // 替换桶数组
}

// 迭代器实现
template <typename K, typename V, typename Hash, typename Enable>
Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::Hhashmap_iterator(Hhashmap<K, V, Hash>* map, int index, ListIterator it)
    : _map(map), _bucket_index(index), _list_iterator(it) {}

template <typename K, typename V, typename Hash, typename Enable>
Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::Hhashmap_iterator()
    : _map(nullptr), _bucket_index(0), _list_iterator(nullptr) {}

template <typename K, typename V, typename Hash, typename Enable>
pair<K, V>& Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::operator*() const {
    return *_list_iterator;
}

template <typename K, typename V, typename Hash, typename Enable>
pair<K, V>* Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::operator->() const {
    return &(*_list_iterator);
}

template <typename K, typename V, typename Hash, typename Enable>
typename Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator& Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::operator++() {
    ++_list_iterator;  // 先移动链表迭代器

    // 若当前链表已遍历完，查找下一个非空桶
    if (_list_iterator == _map->_data[_bucket_index].end()) {
        find_next_valid_bucket();
    }
    return *this;
}

template <typename K, typename V, typename Hash, typename Enable>
typename Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::operator++(int) {
    Hhashmap_iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename K, typename V, typename Hash, typename Enable>
bool Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::operator==(const Hhashmap_iterator& other) const {
    return _bucket_index == other._bucket_index && _list_iterator == other._list_iterator;
}

template <typename K, typename V, typename Hash, typename Enable>
bool Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::operator!=(const Hhashmap_iterator& other) const {
    return !(*this == other);
}

template <typename K, typename V, typename Hash, typename Enable>
void Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator::find_next_valid_bucket() {
    for (++_bucket_index; _bucket_index < _map->_data.size(); ++_bucket_index) {
        if (!_map->_data[_bucket_index].empty()) {
            _list_iterator = _map->_data[_bucket_index].begin();
            return;
        }
    }
    // 所有桶均为空，标记为结束
    _bucket_index = _map->_data.size();
    _list_iterator = ListIterator(nullptr);
}

// 公共成员函数
template <typename K, typename V, typename Hash, typename Enable>
Hhashmap<K, V, Hash, Enable>::Hhashmap() : _data(DEFAULT_INITIAL_CAPACITY), _hash(), _size(0) {}

template <typename K, typename V, typename Hash, typename Enable>
Hhashmap<K, V, Hash, Enable>::Hhashmap(const Hhashmap& other)
    : _data(other._data), _hash(other._hash), _size(other._size) {}

template <typename K, typename V, typename Hash, typename Enable>
Hhashmap<K, V, Hash, Enable>& Hhashmap<K, V, Hash, Enable>::operator=(const Hhashmap& other) {
    if (this != &other) {
        _data = other._data;
        _hash = other._hash;
        _size = other._size;
    }
    return *this;
}

template <typename K, typename V, typename Hash, typename Enable>
bool Hhashmap<K, V, Hash, Enable>::operator==(const Hhashmap& other) const {
    return _data == other._data && _hash == other._hash && _size == other._size;
}

template <typename K, typename V, typename Hash, typename Enable>
bool Hhashmap<K, V, Hash, Enable>::operator!=(const Hhashmap& other) const {
    return !(*this == other);
}

template <typename K, typename V, typename Hash, typename Enable>
int Hhashmap<K, V, Hash, Enable>::get_index(const K& key) const {
    int hash = static_cast<int>(_hash(key));
    return hash & (_data.size() - 1);  // 利用2的幂特性取模
}

template <typename K, typename V, typename Hash, typename Enable>
Llist<pair<K, V>>* Hhashmap<K, V, Hash, Enable>::get_list(int index) {
    return &_data[index];
}

template <typename K, typename V, typename Hash, typename Enable>
const Llist<pair<K, V>>* Hhashmap<K, V, Hash, Enable>::get_list(int index) const {
    return &_data[index];
}

template <typename K, typename V, typename Hash, typename Enable>
void Hhashmap<K, V, Hash, Enable>::insert(const K& key, const V& value) {
    _auto_expand();  // 插入前检查扩容

    int index = get_index(key);
    Llist<pair<K, V>>& list = _data[index];

    // 查找并更新已有元素
    for (typename Llist<pair<K, V>>::Llist_iterator it = list.begin(); it != list.end(); ++it) {
        if (it->first == key) {
            it->second = value;
            return;
        }
    }

    // 插入新元素
    list.add_at_front(pair<K, V>(key, value));
    _size++;
}

template <typename K, typename V, typename Hash, typename Enable>
V* Hhashmap<K, V, Hash, Enable>::get(const K& key) {
    Llist<pair<K, V>>* list = get_list(get_index(key));
    for (typename Llist<pair<K, V>>::Llist_iterator it = list->begin(); it != list->end(); ++it) {
        if (it->first == key) {
            return &(it->second);
        }
    }
    return nullptr;
}

template <typename K, typename V, typename Hash, typename Enable>
const V* Hhashmap<K, V, Hash, Enable>::get(const K& key) const {
    const Llist<pair<K, V>>* list = get_list(get_index(key));
    for (typename Llist<pair<K, V>>::Llist_iterator it = list->begin(); it != list->end(); ++it) {
        if (it->first == key) {
            return &(it->second);
        }
    }
    return nullptr;
}

template <typename K, typename V, typename Hash, typename Enable>
V& Hhashmap<K, V, Hash, Enable>::operator[](const K& key) {
    if (V* val_ptr = get(key)) {
        return *val_ptr;
    }
    throw Eexception("Hhashmap::operator[]: Key not found.");
}

template <typename K, typename V, typename Hash, typename Enable>
int Hhashmap<K, V, Hash, Enable>::size() const {
    return _size;
}

template <typename K, typename V, typename Hash, typename Enable>
int Hhashmap<K, V, Hash, Enable>::empty() const {
    return _size == 0;
}

template <typename K, typename V, typename Hash, typename Enable>
typename Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator Hhashmap<K, V, Hash, Enable>::begin() {
    // 查找第一个非空桶
    for (int i = 0; i < _data.size(); ++i) {
        if (!_data[i].empty()) {
            return Hhashmap_iterator(this, i, _data[i].begin());
        }
    }
    return end();
}

template <typename K, typename V, typename Hash, typename Enable>
typename Hhashmap<K, V, Hash, Enable>::Hhashmap_iterator Hhashmap<K, V, Hash, Enable>::end() {
    return Hhashmap_iterator(this, _data.size(), typename Llist<pair<K, V>>::Llist_iterator(nullptr)); // 模板嵌套需要typename
}

template <typename K, typename V, typename Hash, typename Enable>
void Hhashmap<K, V, Hash, Enable>::clear() {
    for (int i = 0; i < _data.size(); ++i) {
        _data[i].clear();
    }
    _size = 0;
}

template <typename K, typename V, typename Hash, typename Enable>
void Hhashmap<K, V, Hash, Enable>::erase(const K& key) {
    int index = get_index(key);
    Llist<pair<K, V>>* list = get_list(index);

    // 查找并删除元素
    for (typename Llist<pair<K, V>>::Llist_iterator it = list->begin(); it != list->end(); ++it) {
        if (it->first == key) {
            list->remove(*it);
            _size--;
            return;
        }
    }
}

template <typename K, typename V, typename Hash, typename Enable>
V& Hhashmap<K, V, Hash, Enable>::at(const K& key) {
    return operator[](key);
}

template <typename K, typename V, typename Hash, typename Enable>
int Hhashmap<K, V, Hash, Enable>::bucket_size() {
    return _data.size();
}

template <typename K, typename V, typename Hash, typename Enable>
typename Hhashmap<K,V,Hash,Enable>::Hhashmap_iterator Hhashmap<K, V, Hash, Enable>::find(const K& key) {
    int index = get_index(key); // 获取桶索引
    Llist<pair<K, V>>* list = get_list(index); // 获取对应的链表

    for (typename Llist<pair<K, V>>::Llist_iterator it = list->begin(); it != list->end(); ++it) {
        if (it->first == key) {
            return Hhashmap_iterator(this, index, it);
        }
    }
    return end();
}

// -------------------------------------------------------------------
// 偏特化类（红黑树版本）实现
// -------------------------------------------------------------------

// 私有成员函数
template <typename K, typename V, typename Hash>
void Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::_auto_expand() {
    int current_capacity = _data.size();

    // 初始化容量（防止空桶数组）
    if (current_capacity == 0) {
        rehash(DEFAULT_INITIAL_CAPACITY);
        return;
    }

    // 负载因子检查
    if (static_cast<double>(_size) > static_cast<double>(current_capacity) * LOAD_FACTOR) {
        int new_capacity = current_capacity * 2;
        if (new_capacity > 0) {  // 防止溢出
            rehash(new_capacity);
        }
    }
}

template <typename K, typename V, typename Hash>
void Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::rehash(int new_capacity) {
    if (new_capacity <= _data.size()) {
        return;  // 新容量必须大于当前容量
    }

    Aarray<Rred_black_tree<pair<K, V>>> new_buckets(new_capacity);

    // 迁移旧元素到新桶
    int old_capacity = _data.size();
    for (int i = 0; i < old_capacity; ++i) {
        Rred_black_tree<pair<K, V>>& old_tree = _data[i];
        for (typename Rred_black_tree<pair<K, V>>::Rred_black_tree_const_iterator it = old_tree.begin(); it != old_tree.end(); ++it) {
            pair<K, V> p = *it;
            int hash_val = static_cast<int>(_hash(p.first));
            int new_index = hash_val & (new_capacity - 1);  // 利用2的幂特性取模
            new_buckets[new_index].insert(p);
        }
    }

    _data = new_buckets;  // 替换桶数组
}

// 普通迭代器实现
template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::Hhashmap_iterator(
    Hhashmap<K, V, Hash>* map, int index, TreeIterator it)
    : _map(map), _bucket_index(index), _tree_iterator(it) {}

template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::Hhashmap_iterator()
    : _map(nullptr), _bucket_index(0), _tree_iterator(nullptr, nullptr) {}

template <typename K, typename V, typename Hash>
pair<K, V>& Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::operator*() const {
    return *_tree_iterator;
}

template <typename K, typename V, typename Hash>
pair<K, V>* Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::operator->() const {
    return &(*_tree_iterator);
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator&
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::operator++() {
    ++_tree_iterator;  // 先移动红黑树迭代器

    // 若当前树已遍历完，查找下一个非空桶
    if (_tree_iterator == _map->_data[_bucket_index].end()) {
        find_next_valid_bucket();
    }
    return *this;
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::operator++(int) {
    Hhashmap_iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename K, typename V, typename Hash>
bool Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::operator==(const Hhashmap_iterator& other) const {
    return _bucket_index == other._bucket_index && _tree_iterator == other._tree_iterator;
}

template <typename K, typename V, typename Hash>
bool Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::operator!=(const Hhashmap_iterator& other) const {
    return !(*this == other);
}

template <typename K, typename V, typename Hash>
void Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator::find_next_valid_bucket() {
    for (++_bucket_index; _bucket_index < _map->_data.size(); ++_bucket_index) {
        if (!_map->_data[_bucket_index].empty()) {
            _tree_iterator = _map->_data[_bucket_index].begin();
            return;
        }
    }
    // 所有桶均为空，标记为结束
    _bucket_index = _map->_data.size();
    _tree_iterator = TreeIterator(nullptr, nullptr);
}

// 常量迭代器实现
template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::Hhashmap_const_iterator(
    const Hhashmap<K, V, Hash>* map, int index, TreeConstIterator it)
    : _map(map), _bucket_index(index), _tree_iterator(it) {}

template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::Hhashmap_const_iterator()
    : _map(nullptr), _bucket_index(0), _tree_iterator(nullptr, nullptr) {}

template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::Hhashmap_const_iterator(const Hhashmap_iterator& other)
    : _map(other._map), _bucket_index(other._bucket_index), _tree_iterator(other._tree_iterator) {}

template <typename K, typename V, typename Hash>
const pair<K, V>& Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::operator*() const {
    return *_tree_iterator;
}

template <typename K, typename V, typename Hash>
const pair<K, V>* Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::operator->() const {
    return &(*_tree_iterator);
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator&
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::operator++() {
    ++_tree_iterator;  // 先移动红黑树常量迭代器

    // 若当前树已遍历完，查找下一个非空桶
    if (_tree_iterator == _map->_data[_bucket_index].end()) {
        find_next_valid_bucket();
    }
    return *this;
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::operator++(int) {
    Hhashmap_const_iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename K, typename V, typename Hash>
bool Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::operator==(const Hhashmap_const_iterator& other) const {
    return _bucket_index == other._bucket_index && _tree_iterator == other._tree_iterator;
}

template <typename K, typename V, typename Hash>
bool Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::operator!=(const Hhashmap_const_iterator& other) const {
    return !(*this == other);
}

template <typename K, typename V, typename Hash>
void Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator::find_next_valid_bucket() {
    for (++_bucket_index; _bucket_index < _map->_data.size(); ++_bucket_index) {
        if (!_map->_data[_bucket_index].empty()) {
            _tree_iterator = _map->_data[_bucket_index].begin();
            return;
        }
    }
    // 所有桶均为空，标记为结束
    _bucket_index = _map->_data.size();
    _tree_iterator = TreeConstIterator(nullptr, nullptr);
}

// 公共成员函数
template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap()
    : _data(DEFAULT_INITIAL_CAPACITY), _hash(), _size(0) {}

template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap(const Hhashmap& other)
    : _data(other._data), _hash(other._hash), _size(other._size) {}

template <typename K, typename V, typename Hash>
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>&
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::operator=(const Hhashmap& other) {
    if (this != &other) {
        _data = other._data;
        _hash = other._hash;
        _size = other._size;
    }
    return *this;
}

template <typename K, typename V, typename Hash>
bool Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::operator==(const Hhashmap& other) const {
    return _data == other._data && _hash == other._hash && _size == other._size;
}

template <typename K, typename V, typename Hash>
bool Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::operator!=(const Hhashmap& other) const {
    return !(*this == other);
}

template <typename K, typename V, typename Hash>
int Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::get_index(const K& key) const {
    int hash = static_cast<int>(_hash(key));
    return hash & (_data.size() - 1);  // 利用2的幂特性取模
}

template <typename K, typename V, typename Hash>
Rred_black_tree<pair<K, V>>* Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::get_tree(int index) {
    return &_data[index];
}

template <typename K, typename V, typename Hash>
const Rred_black_tree<pair<K, V>>* Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::get_tree(int index) const {
    return &_data[index];
}

template <typename K, typename V, typename Hash>
void Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::insert(const K& key, const V& value) {
    _auto_expand();  // 插入前检查扩容

    int index = get_index(key);
    Rred_black_tree<pair<K, V>>& tree = _data[index];

    // 查找并更新已有元素
    for (typename Rred_black_tree<pair<K, V>>::Rred_black_tree_iterator it = tree.begin(); it != tree.end(); ++it) {
        if (it->first == key) {
            it->second = value;
            return;
        }
    }

    // 插入新元素
    tree.insert(pair<K, V>(key, value));
    _size++;
}

template <typename K, typename V, typename Hash>
V* Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::get(const K& key) {
    Rred_black_tree<pair<K, V>>* tree = get_tree(get_index(key));
    for (typename Rred_black_tree<pair<K, V>>::Rred_black_tree_iterator it = tree->begin(); it != tree->end(); ++it) {
        if (it->first == key) {
            return &(it->second);
        }
    }
    return nullptr;
}

template <typename K, typename V, typename Hash>
const V* Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::get(const K& key) const {
    const Rred_black_tree<pair<K, V>>* tree = get_tree(get_index(key));
    for (typename Rred_black_tree<pair<K, V>>::Rred_black_tree_const_iterator it = tree->begin(); it != tree->end(); ++it) {
        if (it->first == key) {
            return &(it->second);
        }
    }
    return nullptr;
}

template <typename K, typename V, typename Hash>
V& Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::operator[](const K& key) {
    if (V* val_ptr = get(key)) {
        return *val_ptr;
    }
    throw Eexception("Hhashmap::operator[]: Key not found.");
}

template <typename K, typename V, typename Hash>
int Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::size() const {
    return _size;
}

template <typename K, typename V, typename Hash>
int Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::empty() const {
    return _size == 0;
}

template <typename K, typename V, typename Hash>
void Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::clear() {
    for (int i = 0; i < _data.size(); ++i) {
        _data[i].clear();
    }
    _size = 0;
}

template <typename K, typename V, typename Hash>
void Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::erase(const K& key) {
    int index = get_index(key);
    Rred_black_tree<pair<K, V>>* tree = get_tree(index);

    // 查找并删除元素
    for (typename Rred_black_tree<pair<K, V>>::Rred_black_tree_iterator it = tree->begin(); it != tree->end(); ++it) {
        if (it->first == key) {
            tree->remove(*it);
            _size--;
            return;
        }
    }
}

template <typename K, typename V, typename Hash>
V& Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::at(const K& key) {
    return operator[](key);
}

template <typename K, typename V, typename Hash>
int Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::bucket_size() {
    return _data.size();
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::begin() {
    // 查找第一个非空桶
    for (int i = 0; i < _data.size(); ++i) {
        if (!_data[i].empty()) {
            return Hhashmap_iterator(this, i, _data[i].begin());
        }
    }
    return end();
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::end() {
    using Tree = shed_std::Rred_black_tree<shed_std::pair<K, V>>;
    using TreeIterator = typename Tree::Rred_black_tree_iterator;
    return Hhashmap_iterator(this, _data.size(), TreeIterator(nullptr, nullptr));
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::begin() const {
    // 查找第一个非空桶
    for (int i = 0; i < _data.size(); ++i) {
        if (!_data[i].empty()) {
            return Hhashmap_const_iterator(this, i, _data[i].begin());
        }
    }
    return end();
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::end() const {
    using Tree = shed_std::Rred_black_tree<shed_std::pair<K, V>>;
    using TreeConstIterator = typename Tree::Rred_black_tree_const_iterator;
    return Hhashmap_const_iterator(this, _data.size(), TreeConstIterator(nullptr, nullptr));
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::cbegin() const {
    return begin();
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::cend() const {
    return end();
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::find(const K& key) {
    int index = get_index(key); // 获取红黑树
    Rred_black_tree<pair<K,V>>* tree = get_tree(index); // 获取对应的红黑树
    for (typename Rred_black_tree<pair<K, V>>::Rred_black_tree_iterator it = tree->begin(); it != tree->end(); ++it) {
        if (it->first == key) {
            return Hhashmap_iterator(this, index, it);
        }
    }
    return end();
}

template <typename K, typename V, typename Hash>
typename Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::Hhashmap_const_iterator
Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>>::find(const K& key) const{
    int index = get_index(key); // 获取红黑树
    const Rred_black_tree<pair<K,V>>* tree = get_tree(index); // 获取对应的红黑树
    for (typename Rred_black_tree<pair<K, V>>::Rred_black_tree_const_iterator it = tree->begin(); it != tree->end(); ++it) {
        if (it->first == key) {
            return Hhashmap_const_iterator(this, index, it);
        }
    }
    return end();
}

}  // namespace shed_std

#endif  // HHASHMAP_TPP