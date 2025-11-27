#include "Llist.h"
namespace shed_std{
    // Node结构体实现
template <typename T>
    Node<T>::Node() : _next(nullptr) {}

    template <typename T>
    Node<T>::Node(Node* next, T value) : _next(next), _value(value) {}


    // basic_list类实现
    template <typename E>
    basic_list<E>::basic_list() {
        _head = new Node<E>();
        _pre = _head;
    }

    template <typename E>
    basic_list<E>::~basic_list() {
        clear();
        delete _head;
        _head = nullptr;
        _pre = nullptr;
    }

    template <typename E>
    void basic_list<E>::clear() {
        _pre = _head;
        while(_head->_next != nullptr) {
            Node<E>* tmp = _head->_next;
            _head->_next = _head->_next->_next;
            delete tmp;
        }
        _length = 0;
    }

    template <typename E>
    bool basic_list<E>::add(E value) {
        Node<E>* new_node = new Node<E>();
        new_node->_value = value;
        new_node->_next = _pre->_next;
        _pre->_next = new_node;
        _length++;
        return true;
    }

    template <typename E>
    bool basic_list<E>::remove() {
        if(_pre == nullptr) return false;
        if(_pre->_next == nullptr) return false;
        
        Node<E>* tmp = _pre->_next;
        _pre->_next = _pre->_next->_next;
        delete tmp;
        _length--;
        return true;
    }

    template <typename E>
    bool basic_list<E>::move_to_next() {
        if(_pre == nullptr || _pre->_next == nullptr) return false;
        _pre = _pre->_next;
        return true;
    }

    template <typename E>
    bool basic_list<E>::reset_to_head() {
        if(_head == nullptr) return false;
        _pre = _head;
        return true;
    }

    template <typename E>
    bool basic_list<E>::set_cur_value(E value) {
        if(_pre != nullptr && _pre->_next != nullptr) {
            _pre->_next->_value = value;
            return true;
        }
        return false;
    }

    template <typename E>
    bool basic_list<E>::get_cur_value(E& value) const {
        if(_pre != nullptr && _pre->_next != nullptr) {
            value = _pre->_next->_value;
            return true;
        }
        return false;
    }

    template <typename E>
    const int basic_list<E>::length() const {
        return _length;
    }

    template <typename E>
    bool basic_list<E>::empty() const {
        return _length == 0;
    }


    // Llist类实现
    template <typename E>
    Llist<E>::Llist() : basic_list<E>() {}

    template <typename E>
    bool Llist<E>::reverse() {
        if(this->_head == nullptr) return false;
        Node<E>* pre = this->_head->_next;
        if(pre == nullptr) return false;
        Node<E>* cur = pre->_next;
        if(cur == nullptr) return true;

        pre->_next = nullptr;
        Node<E>* next;
        while(cur != nullptr) {
            next = cur->_next;
            cur->_next = pre;
            pre = cur;
            cur = next;
        }
        this->_head->_next = pre;
        return true;
    }


    // Llist_iterator实现
    template <typename E>
    Llist<E>::Llist_iterator::Llist_iterator(Node<E>* node) : _cur(node) {}

    template <typename E>
    E& Llist<E>::Llist_iterator::operator*() const {
        return _cur->_value;
    }

    template <typename E>
    E* Llist<E>::Llist_iterator::operator->() const {
        return &(_cur->_value);
    }

    template <typename E>
    typename Llist<E>::Llist_iterator& Llist<E>::Llist_iterator::operator++() {
        if(_cur != nullptr) {
            _cur = _cur->_next;
        }
        return *this;
    }

    template <typename E>
    typename Llist<E>::Llist_iterator Llist<E>::Llist_iterator::operator++(int) {
        Llist_iterator temp = *this;
        ++(*this);
        return temp;
    }

    template <typename E>
    bool Llist<E>::Llist_iterator::operator==(const Llist_iterator& other) const {
        return _cur == other._cur;
    }

    template <typename E>
    bool Llist<E>::Llist_iterator::operator!=(const Llist_iterator& other) const {
        return !(*this == other);
    }


    // Llist迭代器函数
    template <typename E>
    typename Llist<E>::Llist_iterator Llist<E>::begin() {
        return Llist_iterator(this->_head->_next);
    }

    template <typename E>
    typename Llist<E>::Llist_iterator Llist<E>::begin() const {
        return Llist_iterator(this->_head->_next);
    }

    template <typename E>
    typename Llist<E>::Llist_iterator Llist<E>::end() {
        return Llist_iterator(nullptr);
    }

    template <typename E>
    typename Llist<E>::Llist_iterator Llist<E>::end() const {
        return Llist_iterator(nullptr);
    }


    // Llist其他函数实现
    template <typename E>
    bool Llist<E>::add_after(E value) {
        if(this->_pre == nullptr || this->_pre->_next == nullptr) return false;
        Node<E>* tmp = this->_pre->_next->_next;
        Node<E>* new_node = new Node<E>(tmp, value);
        this->_pre->_next->_next = new_node;
        this->_length++;
        return true;
    }

    template <typename E>
    Llist<E>::Llist(const Llist<E>& other) {
        this->_head = new Node<E>();
        this->_pre = this->_head;
        this->_length = 0;

        Node<E>* tail = this->_head;
        for(Llist_iterator it = other.begin(); it != other.end(); it++) {
            Node<E>* cur = new Node<E>(nullptr, *it);
            tail->_next = cur;
            tail = cur;
            this->_length++;
        }
    }

    template <typename E>
    Llist<E>& Llist<E>::operator=(const Llist<E>& other) {
        if(this == &other) return *this;

        this->clear();
        Node<E>* tail = this->_head;

        for(Llist_iterator it = other.begin(); it != other.end(); it++) {
            Node<E>* cur = new Node<E>(nullptr, *it);
            tail->_next = cur;
            tail = cur;
            this->_length++;
        }

        return *this;
    }

    template <typename E>
    void Llist<E>::add_at_front(E value) {
        Node<E>* new_node = new Node<E>(this->_head->_next, value);
        this->_head->_next = new_node;
        this->_length++;
    }

    template <typename E>
    bool Llist<E>::remove_at_front() {
        if(this->_head->_next == nullptr) return false;
        Node<E>* cur = this->_head->_next;
        this->_head->_next = cur->_next;
        delete cur;
        this->_length--;
        return true;
    }
}