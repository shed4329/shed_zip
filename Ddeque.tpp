#ifndef DDEQUE_TPP
#define DDEQUE_TPP

#include "Ddeque.h"  // 实现依赖声明，需包含头文件

namespace shed_std {

template <typename T>
void Ddeque<T>::_expand() {
    if (_index_front == 0) {
        int old_size = _data.size();
        int expand_size = old_size * _ratio_front;
        Vvector<T> new_vec;
        for (int i = 0; i < expand_size; ++i) {
            new_vec.push_back(T());
        }
        for (int i = 0; i < old_size; ++i) {
            new_vec.push_back(_data[i]);
        }
        _data = new_vec;
        _index_front = expand_size;
        _index_end += expand_size;
    }
}

template <typename T>
Ddeque<T>::Ddeque() {
    int total_size = _size_default + static_cast<int>(_size_default * _ratio_front);
    for (int i = 0; i < total_size; ++i) {
        _data.push_back(T());
    }
    _index_end = _index_front;
}

template <typename T>
Ddeque<T>::Ddeque(int size) {
    int front_size = static_cast<int>(size * _ratio_front);
    int total_size = size + front_size;
    _data.reserve(total_size);
    for (int i = 0; i < front_size; ++i) {
        _data.push_back(T());
    }
    _index_front = front_size;
    for (int i = 0; i < size; ++i) {
        _data.push_back(T());
    }
    _index_end = _index_front + size;
}

template <typename T>
Ddeque<T>::Ddeque(const Ddeque& other) {
    _index_front = other._index_front;
    _index_end = other._index_end;
    _data = other._data;
}

template <typename T>
Ddeque<T>& Ddeque<T>::operator=(const Ddeque& other) {
    if (this == &other) {
        return *this;
    }
    _index_front = other._index_front;
    _index_end = other._index_end;
    _data = other._data;
    return *this;
}

template <typename T>
bool Ddeque<T>::operator==(const Ddeque& other) const {
    if (size() != other.size()) return false;
    for (int i = 0; i < size(); ++i) {
        if (at(i) != other.at(i)) return false;
    }
    return true;
}

template <typename T>
bool Ddeque<T>::operator!=(const Ddeque& other) const {
    return !(*this == other);
}

template <typename T>
int Ddeque<T>::size() const {
    return _index_end - _index_front;
}

template <typename T>
bool Ddeque<T>::empty() const {
    return _index_end == _index_front;
}

template <typename T>
void Ddeque<T>::shrink_to_fit() {
    int new_size = size();
    int new_front = static_cast<int>(new_size * _ratio_front);
    Vvector<T> new_vec;

    for (int i = 0; i < new_front; ++i) {
        new_vec.push_back(T());
    }
    for (int i = _index_front; i < _index_end; ++i) {
        new_vec.push_back(_data[i]);
    }

    _data = new_vec;
    _index_end = new_front + new_size;
    _index_front = new_front;
    _data.shrink_to_fit();
}

template <typename T>
T& Ddeque<T>::at(int index) {
    if (index < 0 || index >= size()) {
        throw Eexception("Eexception: Ddque Index out of range!");
    }
    return _data.at(index + _index_front);
}

template <typename T>
const T& Ddeque<T>::at(int index) const {
    if (index < 0 || index >= size()) {
        throw Eexception("Eexception: Ddque Index out of range!");
    }
    return _data.at(index + _index_front);
}

template <typename T>
T& Ddeque<T>::operator[](int index) {
    return at(index);
}

template <typename T>
const T& Ddeque<T>::operator[](int index) const {
    return at(index);
}

template <typename T>
T& Ddeque<T>::front() {
    if (empty()) {
        throw Eexception("Eexception: empty Ddeque doesn't have front");
    }
    return at(0);
}

template <typename T>
const T& Ddeque<T>::front() const {
    if (empty()) {
        throw Eexception("Eexception: empty Ddeque doesn't have front");
    }
    return at(0);
}

template <typename T>
T& Ddeque<T>::back() {
    if (empty()) {
        throw Eexception("Eexception: empty Ddeque doesn't have back");
    }
    return _data.at(_index_end - 1);
}

template <typename T>
const T& Ddeque<T>::back() const {
    if (empty()) {
        throw Eexception("Eexception: empty Ddeque doesn't have back");
    }
    return _data.at(_index_end - 1);
}

template <typename T>
void Ddeque<T>::clear() {
    int front_space = _index_front;
    _data.clear();
    for (int i = 0; i < front_space; ++i) {
        _data.push_back(T());
    }
    _index_front = front_space;
    _index_end = _index_front;
}

template <typename T>
void Ddeque<T>::insert(int index, T& value) {
    if (index < 0 || index > size()) {
        throw Eexception("Eexception: Ddque Index out of range!");
    }
    int data_index = _index_front + index;
    _data.insert(data_index, value);
    _index_end++;
}

template <typename T>
void Ddeque<T>::erase(int index) {
    if (index < 0 || index >= size()) {
        throw Eexception("Eexception: Ddque Index out of range!");
    }
    int data_index = _index_front + index;
    _data.erase(data_index);
    _index_end--;
}

template <typename T>
void Ddeque<T>::push_back(T& value) {
    _data.insert(_index_end, value);
    _index_end++;
}

template <typename T>
void Ddeque<T>::pop_back() {
    if (!empty()) {
        _data.erase(_index_end - 1);
        _index_end--;
    } else {
        throw Eexception("Eexception: empty Ddeque could not pop back!");
    }
}

template <typename T>
void Ddeque<T>::push_front(T& value) {
    _expand();
    _index_front--;
    _data[_index_front] = value;
}

template <typename T>
void Ddeque<T>::pop_front() {
    if (!empty()) {
        _index_front++;
    } else {
        throw Eexception("Eexception: empty Ddeque could not pop front!");
    }
}

template <typename T>
void Ddeque<T>::swap(int index1, int index2) {
    if (index1 >= 0 && index1 < size() && index2 >= 0 && index2 < size() && index1 != index2) {
        _data.swap(index1 + _index_front, index2 + _index_front);
    } else {
        throw Eexception("Eexception: Ddque swap index out of range!");
    }
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::begin() {
    return Ddeque_iterator(&_data, _index_front);
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::begin() const {
    return Ddeque_const_iterator(&_data, _index_front);
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::end() {
    return Ddeque_iterator(&_data, _index_end);
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::end() const {
    return Ddeque_const_iterator(&_data, _index_end);
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::rbegin() {
    return Ddeque_iterator(&_data, _index_end - 1);
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::rbegin() const {
    return Ddeque_const_iterator(&_data, _index_end - 1);
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::rend() {
    return Ddeque_iterator(&_data, _index_front - 1);
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::rend() const {
    return Ddeque_const_iterator(&_data, _index_front - 1);
}

template <typename T>
int Ddeque<T>::find_first(const T& value) const {
    int data_idx = _data.find_first(value, _index_front, _index_end);
    return (data_idx == -1) ? -1 : (data_idx - _index_front);
}

template <typename T>
int Ddeque<T>::find_first(const T& value, int start_index, int end_index) const {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return -1;
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    int data_idx = _data.find_first(value, data_start, data_end);
    return (data_idx == -1) ? -1 : (data_idx - _index_front);
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::find_first(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return end();
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    auto it = _data.find_first(predicate, data_start, data_end);
    return (it == _data.end()) ? end() : Ddeque_iterator(&_data, it - _data.begin());
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::find_first(const Ffunction<bool, const T&>& predicate) {
    return find_first(predicate, 0, size());
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::find_first(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) const {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return end();
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    auto it = _data.find_first(predicate, data_start, data_end);
    return (it == _data.end()) ? end() : Ddeque_const_iterator(&_data, it - _data.begin());
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::find_first(const Ffunction<bool, const T&>& predicate) const {
    return find_first(predicate, 0, size());
}

template <typename T>
bool Ddeque<T>::contains(const T& value) const {
    return _data.contains(value, _index_front, _index_end);
}

template <typename T>
bool Ddeque<T>::contains(const T& value, int start_index, int end_index) const {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return false;
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    return _data.contains(value, data_start, data_end);
}

template <typename T>
bool Ddeque<T>::contains(const Ffunction<bool, const T&>& predicate) const {
    return _data.contains(predicate, _index_front, _index_end);
}

template <typename T>
bool Ddeque<T>::contains(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) const {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return false;
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    return _data.contains(predicate, data_start, data_end);
}

template <typename T>
int Ddeque<T>::find_last(const T& value) const {
    int data_idx = _data.find_last(value, _index_front, _index_end);
    return (data_idx == -1) ? -1 : (data_idx - _index_front);
}

template <typename T>
int Ddeque<T>::find_last(const T& value, int start_index, int end_index) const {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return -1;
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    int data_idx = _data.find_last(value, data_start, data_end);
    return (data_idx == -1) ? -1 : (data_idx - _index_front);
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::find_last(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return end();
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    auto it = _data.find_last(predicate, data_start, data_end);
    return (it == _data.end()) ? Ddeque_iterator(&_data, data_end) : Ddeque_iterator(&_data, it - _data.begin());
}

template <typename T>
typename Ddeque<T>::Ddeque_iterator Ddeque<T>::find_last(const Ffunction<bool, const T&>& predicate) {
    return find_last(predicate, 0, size());
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::find_last(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) const {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return end();
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    auto it = _data.find_last(predicate, data_start, data_end);
    return (it == _data.end()) ? Ddeque_const_iterator(&_data, data_end) : Ddeque_const_iterator(&_data, it - _data.begin());
}

template <typename T>
typename Ddeque<T>::Ddeque_const_iterator Ddeque<T>::find_last(const Ffunction<bool, const T&>& predicate) const {
    return find_last(predicate, 0, size());
}

template <typename T>
void Ddeque<T>::fill(const T& value) {
    _data.fill(value, _index_front, _index_end);
}

template <typename T>
void Ddeque<T>::fill(const T& value, int start_index, int end_index) {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        return;
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    _data.fill(value, data_start, data_end);
}

template <typename T>
void Ddeque<T>::reverse() {
    int left = _index_front;
    int right = _index_end - 1;
    while (left < right) {
        _data.swap(left, right);
        left++;
        right--;
    }
}

template <typename T>
Ddeque<T> Ddeque<T>::subDeque(int start_index, int end_index) {
    if (start_index < 0 || end_index > size() || start_index >= end_index) {
        throw Eexception("Exception: Invalid Interval (subVec)");
    }
    int data_start = _index_front + start_index;
    int data_end = _index_front + end_index;
    Vvector<T> sub_vvec = _data.subVec(data_start, data_end);

    Ddeque<T> sub_ddeque;
    sub_ddeque._data.clear();
    sub_ddeque._index_front = 0;
    for (int i = 0; i < sub_vvec.size(); ++i) {
        sub_ddeque._data.push_back(sub_vvec[i]);
    }
    sub_ddeque._index_end = sub_vvec.size();
    return sub_ddeque;
}

}  // namespace shed_std

#endif  // DDEQUE_TPP