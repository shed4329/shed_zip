#ifndef HHEAP_TPP
#define HHEAP_TPP

#include "Hheap.h"

namespace shed_std {

template <typename T, typename Comparator>
void Hheap<T, Comparator>::shift_up(int index) {
    if (index <= 0) {
        // 索引不合法（已到根节点），无需调整
        return;
    }
    // 计算父节点索引
    int parent = (index - 1) / 2;
    // 若子节点优先级高于父节点（根据比较器判断），则交换并继续向上调整
    if (_comparator(_data[parent], _data[index])) {
        _data.swap(index, parent);
        shift_up(parent);
    }
}

template <typename T, typename Comparator>
void Hheap<T, Comparator>::shift_down(int index) {
    int left = 2 * index + 1;   // 左子节点索引
    int right = 2 * index + 2;  // 右子节点索引
    int target = index;         // 记录需要交换的目标节点索引（初始为当前节点）

    // 比较左子节点与当前节点，更新目标节点
    if (left < _data.size() && _comparator(_data[target], _data[left])) {
        target = left;
    }
    // 比较右子节点与当前目标节点，更新目标节点
    if (right < _data.size() && _comparator(_data[target], _data[right])) {
        target = right;
    }

    // 若目标节点不是当前节点，则交换并继续向下调整
    if (target != index) {
        _data.swap(index, target);
        shift_down(target);
    }
}

template <typename T, typename Comparator>
Hheap<T, Comparator>::Hheap() : _data(), _comparator(Comparator()) {}

template <typename T, typename Comparator>
Hheap<T, Comparator>::Hheap(const Comparator& comparator) : _data(), _comparator(comparator) {}

template <typename T, typename Comparator>
Hheap<T, Comparator>::Hheap(const Vvector<T>& vec, const Comparator& comparator) 
    : _data(vec), _comparator(comparator) {
    // 从最后一个非叶子节点开始堆化（最后一个非叶子节点索引：size/2 - 1）
    for (int i = (_data.size() / 2) - 1; i >= 0; --i) {
        shift_down(i);
    }
}

template <typename T, typename Comparator>
bool Hheap<T, Comparator>::operator==(const Hheap& other) const {
    return _data == other._data && _comparator == other._comparator;
}

template <typename T, typename Comparator>
bool Hheap<T, Comparator>::operator!=(const Hheap& other) const {
    return !(*this == other);
}

template <typename T, typename Comparator>
int Hheap<T, Comparator>::size() const {
    return _data.size();
}

template <typename T, typename Comparator>
bool Hheap<T, Comparator>::empty() const {
    return _data.empty();
}

template <typename T, typename Comparator>
const T& Hheap<T, Comparator>::top() const {
    if (empty()) {
        throw Eexception("Hheap: top from empty heap");
    }
    return _data[0];
}

template <typename T, typename Comparator>
void Hheap<T, Comparator>::push(const T& value) {
    _data.push_back(value);
    // 对新插入的元素（位于末尾）进行向上调整
    shift_up(_data.size() - 1);
}

template <typename T, typename Comparator>
void Hheap<T, Comparator>::pop() {
    if (empty()) {
        throw Eexception("Hheap: pop from empty heap");
    }
    // 将堆顶元素替换为最后一个元素
    _data[0] = _data.back();
    _data.pop_back();
    // 若堆不为空，对新的堆顶元素进行向下调整
    if (!empty()) {
        shift_down(0);
    }
}

template <typename T, typename Comparator>
void Hheap<T, Comparator>::clear() {
    while (!empty()) {
        pop();
    }
}

template <typename T, typename Comparator>
const Comparator& Hheap<T, Comparator>::get_comparator() const {
    return _comparator;
}

} // namespace shed_std

#endif // HHEAP_TPP