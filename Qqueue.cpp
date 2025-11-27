#include "Qqueue.h"
namespace shed_std{
        // ------------------------------
    // 类外：静态成员初始化
    // ------------------------------
    template <typename T>
    const double Qqueue<T>::_trim_ratio = 0.5;

    // ------------------------------
    // 类外：私有成员函数实现 _trim
    // ------------------------------
    template <typename T>
    void Qqueue<T>::_trim() {
        // 当偏移量占总容量比例超过整理阈值时，清理冗余元素
        if (_offset > static_cast<int>(_data.size() * _trim_ratio)) {
            Vvector<T> new_data;
            // 复制有效元素（从 _offset 到数组末尾）
            for (int i = _offset; i < _data.size(); ++i) {
                new_data.push_back(_data[i]);
            }
            // 替换原数组，重置偏移量
            _data = new_data;
            _offset = 0;
        }
    }

    // ------------------------------
    // 类外：构造函数实现
    // ------------------------------
    template <typename T>
    Qqueue<T>::Qqueue(const Qqueue& other) {
        // 仅复制 other 的有效元素（跳过 other._offset 前的冗余）
        for (int i = other._offset; i < other._data.size(); ++i) {
            _data.push_back(other._data[i]);
        }
        _offset = 0;  // 新队列无冗余元素，偏移量重置
    }

    // ------------------------------
    // 类外：赋值运算符实现
    // ------------------------------
    template <typename T>
    Qqueue<T>& Qqueue<T>::operator=(const Qqueue& other) {
        if (this != &other) {  // 避免自赋值
            _data.clear();     // 清空当前元素
            _offset = 0;       // 重置偏移量
            // 复制 other 的有效元素
            for (int i = other._offset; i < other._data.size(); ++i) {
                _data.push_back(other._data[i]);
            }
        }
        return *this;
    }

    // ------------------------------
    // 类外：公有成员函数实现
    // ------------------------------
    template <typename T>
    void Qqueue<T>::push(const T& value) {
        _data.push_back(value);
    }

    template <typename T>
    void Qqueue<T>::pop() {
        if (empty()) {
            throw Eexception("Eexception: empty Qqueue could not pop!");
        }
        _offset++;    // 逻辑删除队头
        _trim();      // 检查是否需要整理
    }

    template <typename T>
    T& Qqueue<T>::front() {
        if (empty()) {
            throw Eexception("Eexception: empty Qqueue has no front!");
        }
        return _data[_offset];
    }

    template <typename T>
    const T& Qqueue<T>::front() const {
        if (empty()) {
            throw Eexception("Eexception: empty Qqueue has no front!");
        }
        return _data[_offset];
    }

    template <typename T>
    T& Qqueue<T>::back() {
        if (empty()) {
            throw Eexception("Eexception: empty Qqueue has no back!");
        }
        return _data.back();
    }

    template <typename T>
    const T& Qqueue<T>::back() const {
        if (empty()) {
            throw Eexception("Eexception: empty Qqueue has no back!");
        }
        return _data.back();
    }

    template <typename T>
    bool Qqueue<T>::empty() const {
        return _data.size() == _offset;
    }

    template <typename T>
    int Qqueue<T>::size() const {
        return static_cast<int>(_data.size()) - _offset;
    }

    template <typename T>
    void Qqueue<T>::clear() {
        _data.clear();
        _offset = 0;
    }

    template <typename T>
    bool Qqueue<T>::operator==(const Qqueue& other) const {
        if (size() != other.size()) {
            return false;
        }
        // 逐元素比较有效元素
        for (int i = 0; i < size(); ++i) {
            if (_data[_offset + i] != other._data[other._offset + i]) {
                return false;
            }
        }
        return true;
    }

    template <typename T>
    bool Qqueue<T>::operator!=(const Qqueue& other) const {
        return !(*this == other);
    }
}
    