#ifndef VVECTOR_TPP
#define VVECTOR_TPP

#include "Vvector.h"

template <typename E>
shed_std::Vvector<E>::Vvector_iterator::Vvector_iterator(Vvector<E>* vec, int index) {
    if (index < -1 || index > vec->_size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    _vec = vec;
    _index = index;
}

template <typename E>
E& shed_std::Vvector<E>::Vvector_iterator::operator*() {
    return _vec->at(_index);
}

template <typename E>
E* shed_std::Vvector<E>::Vvector_iterator::operator->() {
    return &(operator*());
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator& shed_std::Vvector<E>::Vvector_iterator::operator++() {
    if (_index + 1 > _vec->_size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    _index++;
    return *this;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::Vvector_iterator::operator++(int) {
    Vvector_iterator tmp = *this;
    ++(*this);
    return tmp;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator& shed_std::Vvector<E>::Vvector_iterator::operator--() {
    if (_index - 1 < -1) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    _index--;
    return *this;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::Vvector_iterator::operator--(int) {
    Vvector_iterator tmp = *this;
    --(*this);
    return tmp;
}

template <typename E>
bool shed_std::Vvector<E>::Vvector_iterator::operator==(const Vvector_iterator& other) const {
    return _index == other._index && _vec == other._vec;
}

template <typename E>
bool shed_std::Vvector<E>::Vvector_iterator::operator!=(const Vvector_iterator& other) const {
    return !(*this == other);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::Vvector_iterator::operator+(int n) const {
    int new_index = _index + n;
    if (new_index < -1 || new_index > _vec->_size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    return Vvector_iterator(_vec, new_index);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::Vvector_iterator::operator-(int n) const {
    return operator+(-n);
}

template <typename E>
int shed_std::Vvector<E>::Vvector_iterator::operator-(const Vvector_iterator& other) const {
    return _index - other._index;
}

template <typename E>
shed_std::Vvector<E>::Vvector_const_iterator::Vvector_const_iterator(const Vvector<E>* vec, int index) {
    if (index < -1 || index > vec->_size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    _vec = vec;
    _index = index;
}

template <typename E>
const E& shed_std::Vvector<E>::Vvector_const_iterator::operator*() const {
    return _vec->at(_index);
}

template <typename E>
const E* shed_std::Vvector<E>::Vvector_const_iterator::operator->() const {
    return &(operator*());
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator& shed_std::Vvector<E>::Vvector_const_iterator::operator++() {
    if (_index + 1 > _vec->_size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    _index++;
    return *this;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::Vvector_const_iterator::operator++(int) {
    Vvector_const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator& shed_std::Vvector<E>::Vvector_const_iterator::operator--() {
    if (_index - 1 < -1) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    _index--;
    return *this;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::Vvector_const_iterator::operator--(int) {
    Vvector_const_iterator tmp = *this;
    --(*this);
    return tmp;
}

template <typename E>
bool shed_std::Vvector<E>::Vvector_const_iterator::operator==(const Vvector_const_iterator& other) const {
    return _index == other._index && _vec == other._vec;
}

template <typename E>
bool shed_std::Vvector<E>::Vvector_const_iterator::operator!=(const Vvector_const_iterator& other) const {
    return !(*this == other);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::Vvector_const_iterator::operator+(int n) const {
    int new_index = _index + n;
    if (new_index < -1 || new_index > _vec->_size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    return Vvector_const_iterator(_vec, new_index);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::Vvector_const_iterator::operator-(int n) const {
    return operator+(-n);
}

template <typename E>
int shed_std::Vvector<E>::Vvector_const_iterator::operator-(const Vvector_const_iterator& other) const {
    return _index - other._index;
}

template <typename E>
shed_std::Vvector<E>::Vvector() : _capacity(1024), _size(0), _array(1024) {}

template <typename E>
shed_std::Vvector<E>::Vvector(int size) : _capacity(_get_fitting_capacity(size)), _size(size), _array(_capacity) {
    for(int i = 0;i<size;++i){
        _array[i] = E();
    }
}

template <typename E>
shed_std::Vvector<E>::Vvector(const Vvector& other) : _size(other._size), _capacity(other._capacity), _array(other._array) {}

template <typename E>
shed_std::Vvector<E>& shed_std::Vvector<E>::operator=(const Vvector& other) {
    if (this == &other) {
        return *this;
    }
    _size = other._size;
    _capacity = other._capacity;
    _array = other._array;
    return *this;
}

template <typename E>
bool shed_std::Vvector<E>::operator==(const Vvector& other) const {
    if (_size != other._size) {
        return false;
    }
    for (int i = 0; i < _size; i++) {
        if (_array[i] != other[i]) {
            return false;
        }
    }
    return true;
}

template <typename E>
bool shed_std::Vvector<E>::operator!=(const Vvector& other) const {
    return !(*this == other);
}

template <typename E>
int shed_std::Vvector<E>::length() const {
    return _size;
}

template <typename E>
int shed_std::Vvector<E>::size() const {
    return _size;
}

template <typename E>
bool shed_std::Vvector<E>::empty() const {
    return _size == 0;
}

template <typename E>
E& shed_std::Vvector<E>::at(int index) {
    if (index < 0 || index >= _size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    return _array[index];
}

template <typename E>
const E& shed_std::Vvector<E>::at(int index) const {
    if (index < 0 || index >= _size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    return _array[index];
}

template <typename E>
E& shed_std::Vvector<E>::operator[](int index) {
    return at(index);
}

template <typename E>
const E& shed_std::Vvector<E>::operator[](int index) const {
    return at(index);
}

template <typename E>
void shed_std::Vvector<E>::push_back(const E& value) {
    if (_size == MAX_SIZE) {
        throw Eexception("Eexception: Vvector reach max size limits!");
    }
    if (_size + 1 > _capacity) {
        _expand();
    }
    _size++;
    _array[_size - 1] = value;
}

template <typename E>
void shed_std::Vvector<E>::pop_back() {
    if (_size == 0) {
        throw Eexception("Exception:empty Vvector could not pop!");
    }
    _size--;
}

template <typename E>
void shed_std::Vvector<E>::insert(int index, const E& value) {
    if (index < 0 || index > _size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    if (_size == MAX_SIZE) {
        throw Eexception("Eexception: Vvector reach max size limits!");
    }
    if (_size + 1 > _capacity) {
        _expand();
    }
    _size++;
    for (int i = _size; i > index; i--) {
        _array[i] = _array[i - 1];
    }
    _array[index] = value;
}

template <typename E>
void shed_std::Vvector<E>::erase(int index) {
    if (index < 0 || index >= _size) {
        throw Eexception("Eexception: Vvector index out of range");
    }
    _size--;
    for (int i = index; i < _size; i++) {
        _array[i] = _array[i + 1];
    }
}

template <typename E>
E& shed_std::Vvector<E>::front() {
    if (_size == 0) {
        throw Eexception("Eexception: empty Vvector doesn't have front");
    }
    return _array[0];
}

template <typename E>
const E& shed_std::Vvector<E>::front() const {
    if (_size == 0) {
        throw Eexception("Eexception: empty Vvector doesn't have front");
    }
    return _array[0];
}

template <typename E>
E& shed_std::Vvector<E>::back() {
    if (_size == 0) {
        throw Eexception("Eexception: empty Vvector doesn't have back");
    }
    return _array[_size - 1];
}

template <typename E>
const E& shed_std::Vvector<E>::back() const {
    if (_size == 0) {
        throw Eexception("Eexception: empty Vvector doesn't have back");
    }
    return _array[_size - 1];
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::begin() {
    return Vvector_iterator(this, 0);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::begin() const {
    return Vvector_const_iterator(this, 0);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::end() {
    return Vvector_iterator(this, _size);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::end() const {
    return Vvector_const_iterator(this, _size);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::rbegin() {
    return Vvector_iterator(this, _size - 1);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::rbegin() const {
    return Vvector_const_iterator(this, _size - 1);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::rend() {
    return Vvector_iterator(this, -1);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::rend() const {
    return Vvector_const_iterator(this, -1);
}

template <typename E>
void shed_std::Vvector<E>::clear() {
    _size = 0;
}

template <typename E>
int shed_std::Vvector<E>::max_size() const {
    return MAX_SIZE;
}

template <typename E>
int shed_std::Vvector<E>::capacity() const {
    return _capacity;
}

template <typename E>
void shed_std::Vvector<E>::shrink_to_fit() {
    int new_capacity = _get_fitting_capacity(_size);
    if (new_capacity < _capacity) {
        Aarray<E> new_array(new_capacity);
        for (int i = 0; i < _size; i++) {
            new_array[i] = _array[i];
        }
        _array = new_array;
        _capacity = new_capacity;
    }
}

template <typename E>
void shed_std::Vvector<E>::reserve(int size) {
    int new_capacity = _get_fitting_capacity(size);
    if (new_capacity > _capacity) {
        Aarray<E> new_array(new_capacity);
        for (int i = 0; i < _size; i++) {
            new_array[i] = _array[i];
        }
        _array = new_array;
        _capacity = new_capacity;
    }
}

template <typename E>
int shed_std::Vvector<E>::find_first(const E& value) const {
    return _array.find_first(value, 0, _size);
}

template <typename E>
int shed_std::Vvector<E>::find_first(const E& value, int start_index, int end_index) const {
    if (!_is_valid_range(start_index, end_index)) {
        return -1;
    }
    return _array.find_first(value, start_index, end_index);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::find_first(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) {
    if (!_is_valid_range(start_index, end_index)) {
        return end();
    }
    Vvector_iterator it(this, start_index);
    Vvector_iterator endit(this, end_index);
    for (; it != endit; ++it) {
        if (predicate(*it)) {
            break;
        }
    }
    return it;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::find_first(const Ffunction<bool, const E&>& predicate) {
    return find_first(predicate, 0, _size);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::find_first(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) const {
    if (!_is_valid_range(start_index, end_index)) {
        return end();
    }
    Vvector_const_iterator it(this, start_index);
    Vvector_const_iterator endit(this, end_index);
    for (; it != endit; ++it) {
        if (predicate(*it)) {
            break;
        }
    }
    return it;
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::find_first(const Ffunction<bool, const E&>& predicate) const {
    return find_first(predicate, 0, _size);
}

template <typename E>
bool shed_std::Vvector<E>::contains(const E& value) const {
    return find_first(value) != -1;
}

template <typename E>
bool shed_std::Vvector<E>::contains(const E& value, int start_index, int end_index) const {
    return find_first(value, start_index, end_index) != -1;
}

template <typename E>
bool shed_std::Vvector<E>::contains(const Ffunction<bool, const E&>& predicate) const {
    return find_first(predicate) != end();
}

template <typename E>
bool shed_std::Vvector<E>::contains(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) const {
    Vvector_const_iterator end_it(this, end_index);
    Vvector_const_iterator ans = find_first(predicate, start_index, end_index);
    return (ans != end() && ans != end_it);
}

template <typename E>
int shed_std::Vvector<E>::find_last(const E& value) const {
    return _array.find_last(0, _size, value);
}

template <typename E>
int shed_std::Vvector<E>::find_last(const E& value, int start_index, int end_index) const {
    if (!_is_valid_range(start_index, end_index)) {
        return -1;
    }
    return _array.find_last(value, start_index, end_index);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::find_last(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) {
    if (!_is_valid_range(start_index, end_index)) {
        return end();
    }
    Vvector_iterator it(this, end_index - 1);
    Vvector_iterator end_it(this, start_index);
    while (true) {
        if (predicate(*it)) {
            return it;
        }
        if (it == end_it) {
            break;
        }
        --it;
    }
    return Vvector_iterator(this, end_index);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_iterator shed_std::Vvector<E>::find_last(const Ffunction<bool, const E&>& predicate) {
    return find_last(predicate, 0, _size);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::find_last(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) const {
    if (!_is_valid_range(start_index, end_index)) {
        return end();
    }
    Vvector_const_iterator it(this, end_index - 1);
    Vvector_const_iterator end_it(this, start_index);
    while (true) {
        if (predicate(*it)) {
            return it;
        }
        if (it == end_it) {
            break;
        }
        --it;
    }
    return Vvector_const_iterator(this, end_index);
}

template <typename E>
typename shed_std::Vvector<E>::Vvector_const_iterator shed_std::Vvector<E>::find_last(const Ffunction<bool, const E&>& predicate) const {
    return find_last(predicate, 0, _size);
}

template <typename E>
void shed_std::Vvector<E>::fill(const E& value) {
    _array.fill(value, 0, _size);
}

template <typename E>
void shed_std::Vvector<E>::fill(const E& value, int start_index, int end_index) {
    if (_is_valid_range(start_index, end_index)) {
        _array.fill(value, start_index, end_index);
    }
}

template <typename E>
void shed_std::Vvector<E>::swap(int index1, int index2) {
    if (index1 >= 0 && index1 < _size && index2 >= 0 && index2 < _size && index1 != index2) {
        _array.swap(index1, index2);
    }
}

template <typename E>
void shed_std::Vvector<E>::reverse() {
    if (_size <= 1) {
        return;
    }
    for (int i = 0, j = _size - 1; i < j; i++, j--) {
        _array.swap(i, j);
    }
}

template <typename E>
shed_std::Vvector<E> shed_std::Vvector<E>::subVec(int start_index, int end_index) {
    if (!_is_valid_range(start_index, end_index)) {
        throw Eexception("Exception: Invalid Interval");
    }
    int new_size = end_index - start_index;
    Vvector<E> new_Vvector(new_size);
    for (int i = start_index, j = 0; i < end_index; i++, j++) {
        new_Vvector[j] = _array[i];
    }
    return new_Vvector;
}

template <typename E>
int shed_std::Vvector<E>::_get_fitting_capacity(int size) {
    if (size < 0 || size > MAX_CAPACITY) {
        throw Eexception("Eexception:could not allocate a Vvector with Illegal size");
    }
    if (size == 0) {
        return 1;
    }
    if (size == MAX_SIZE) {
        return MAX_CAPACITY;
    }
    int ans = 1;
    while (ans < size) {
        ans <<= 1;
    }
    return ans;
}

template <typename E>
void shed_std::Vvector<E>::_expand() {
    if (_capacity > (MAX_CAPACITY >> 1)) {
        throw Eexception("Eexception:could not expand a Vvector for exceeding size limit");
    }
    int new_cap = _capacity << 1;
    Aarray<E> new_array(new_cap);
    for (int i = 0; i < _size; i++) {
        new_array[i] = _array[i];
    }
    _array = new_array;
    _capacity = new_cap;
}

template <typename E>
bool shed_std::Vvector<E>::_is_valid_range(int start, int end) const {
    return start >= 0 && end >= 0 && start <= _size && end <= _size && start < end;
}

template <typename E>
void shed_std::Vvector<E>::resize(int count) {
    // 1. 防御性检查：拦截负数
    if (count < 0) {
        throw shed_std::Eexception("Eexception: Vvector resize() get a negative!");
    }

    // 2. 缩小尺寸：析构多余元素（核心补充）
    if (count < size()) {
        // 仅析构 [count, size()) 区间的元素，释放对象资源
        for (int i = count; i < size(); ++i) {
            _array[i].~E();
        }
    }

    // 3. 确保容量足够：扩容（时机调整到析构后，避免无效迁移）
    reserve(count);

    // 4. 扩大尺寸
    if (count > size()) {
        for (int i = size(); i < count; ++i) {
            _array[i] = E();
        }
    }

    // 5. 更新尺寸
    _size = count;
}

#endif // VVECTOR_TPP