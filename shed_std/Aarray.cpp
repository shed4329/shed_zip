#include "Aarray.h"

namespace shed_std{
    template <typename T>
    basic_array<T>::basic_array(int N){
        _array = (N > 0) ? new T[N] : nullptr;
        _length = N;
    }

    template <typename T>
    basic_array<T>::~basic_array(){
        delete[] _array;
        _array = nullptr;
        _length = 0;
    }

    template <typename T>
    T& basic_array<T>::operator[] (int index){
        return _array[index];
    }
    
    template <typename T>
    const T& basic_array<T>::operator[](int index) const{
        return _array[index];
    }
    
    template <typename T>
    bool basic_array<T>::at(int index, T& return_value){
        if(index<0||index>=_length){
            return false;
        }
        return_value = _array[index];
        return true;
    }

    template <typename T>
    bool basic_array<T>::at(int index, T& return_value) const{
        if(index<0||index>=_length){
            return false;
        }
        return_value = _array[index];
        return true;
    }

    template <typename T>
    T& basic_array<T>::front(){
        return _array[0];
    }

    template <typename T>
    const T& basic_array<T>::front() const{
        return _array[0];
    }

    template <typename T>
    T& basic_array<T>::back(){
        return _array[_length-1];
    }

    template <typename T>
    const T& basic_array<T>::back() const{
        return _array[_length-1];
    }

    template <typename T>
    const int basic_array<T>::length() const{
        return _length;
    }

    template <typename T>
    bool basic_array<T>::empty() const{
        return _length == 0;
    }

    template<typename E>
    Aarray<E>::Aarray(int N):basic_array<E>(N){
        if(N<0){
            throw Eexception("Exception: Illegal Size for the size of Aarray could not be less than 0!");
        }
    }

    template<typename E>
    Aarray<E>::Aarray(const Aarray& other):basic_array<E>(0){
        this->_length = other._length;
        if(this->_length == 0){
            this->_array = nullptr;
            return;
        }
        this->_array = new  E[this->_length];
        // 拷贝
        Aarray_const_iterator it = other.begin();
        int i = 0;
        while(it!=other.end()){
            this->_array[i] = *it;
            it++;
            i++;
        }
    }

    template<typename E>
    Aarray<E>& Aarray<E>::operator= (const Aarray& other){
        if(this==&other){
            return *this;
        }
        // 删除当前数组
        delete[] this->_array;
        // 赋值
        this->_length = other._length;
        if(this->_length == 0){
            this->_array = nullptr;
            return *this;
        }
        this->_array = new  E[this->_length];
        // 拷贝
        Aarray_const_iterator it = other.begin();
        int i = 0;
        while(it!=other.end()){
            this->_array[i] = *it;
            it++;
            i++;
        }
        return *this;
    }

    template<typename E>
    E& Aarray<E>::at(int index){
        if(index<0||index>=this->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        return this->_array[index];
    }
    
    template<typename E>
    const E& Aarray<E>::at(int index) const{
        if(index<0||index>=this->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        return this->_array[index];
    }
    
    template<typename E>
    E& Aarray<E>::front(){
        if(this->empty()){
            throw Eexception("Exception: Null Pointer Reference for the Aarray's size is 0!");
        }
        return this->_array[0];
    }

    template<typename E>
    const E& Aarray<E>::front() const{
        if(this->empty()){
            throw Eexception("Exception: Null Pointer Reference for the Aarray's size is 0!");
        }
        return this->_array[0];
    }
    
    template<typename E>
    E& Aarray<E>::back(){
        if(this->empty()){
            throw Eexception("Exception: Null Pointer Reference for the Aarray's size is 0!");
        }
        return this->_array[this->_length - 1];
    }

    template<typename E>
    const E& Aarray<E>::back() const{
        if(this->empty()){
            throw Eexception("Exception: Null Pointer Reference for the Aarray's size is 0!");
        }
        return this->_array[this->_length - 1];
    }

    // ------------------------------ Aarray_iterator 实现 ------------------------------
    template<typename E>
    Aarray<E>::Aarray_iterator::Aarray_iterator(int index,Aarray* array):_index(index),_array(array){
        if(_index < 0 ||_index > _array->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
    }

    template<typename E>
    E& Aarray<E>::Aarray_iterator::operator*() const{
        return (*_array).at(_index);
    }

    template<typename E>
    E* Aarray<E>::Aarray_iterator::operator->() const{
        return &((*_array).at(_index));
    }

    template<typename E>
    typename Aarray<E>::Aarray_iterator& Aarray<E>::Aarray_iterator::operator++(){
        if(_index+1 > _array->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        _index++;
        return *this;
    }

    template<typename E>
    typename Aarray<E>::Aarray_iterator Aarray<E>::Aarray_iterator::operator++(int){
        Aarray_iterator temp = *this;
        ++(*this);
        return temp;
    }

    template<typename E>
    typename Aarray<E>::Aarray_iterator& Aarray<E>::Aarray_iterator::operator--(){
        if(_index-1 <0){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        _index--;
        return *this;
    }

    template<typename E>
    typename Aarray<E>::Aarray_iterator Aarray<E>::Aarray_iterator::operator--(int){
        Aarray_iterator temp = *this;
        --(*this);
        return temp;
    }

    template<typename E>
    typename Aarray<E>::Aarray_iterator Aarray<E>::Aarray_iterator::operator+(int n) const{
        int new_index = _index + n;
        if(new_index <0 || new_index > _array->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        return Aarray_iterator(new_index,_array);
    }
    
    template<typename E>
    typename Aarray<E>::Aarray_iterator Aarray<E>::Aarray_iterator::operator-(int n) const{
        return (*this) + (-n);
    }

    template<typename E>
    int Aarray<E>::Aarray_iterator::operator-(const Aarray_iterator& other) const{
        if(_array != other._array){
            throw Eexception("Exception: substract operator cannot be carried on different Aarray");
        }
        return _index - other._index;
    }

    template<typename E>
    bool Aarray<E>::Aarray_iterator::operator==(const Aarray_iterator& other) const{
        return _index == other._index && _array == other._array;
    }

    template<typename E>
    bool Aarray<E>::Aarray_iterator::operator!=(const Aarray_iterator& other) const{
        return !(*this == other);
    }

    // ------------------------------ Aarray_const_iterator 实现 ------------------------------
    template<typename E>
    Aarray<E>::Aarray_const_iterator::Aarray_const_iterator(int index,const Aarray* array):_index(index),_array(array){
        if(_index < 0 ||_index > _array->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
    }

    template<typename E>
    const E& Aarray<E>::Aarray_const_iterator::operator*() const{
        return (*_array).at(_index);
    }

    template<typename E>
    const E* Aarray<E>::Aarray_const_iterator::operator->() const{
        return &((*_array).at(_index));
    }

    template<typename E>
    typename Aarray<E>::Aarray_const_iterator& Aarray<E>::Aarray_const_iterator::operator++(){
        if(_index+1 > _array->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        _index++;
        return *this;
    }

    template<typename E>
    typename Aarray<E>::Aarray_const_iterator Aarray<E>::Aarray_const_iterator::operator++(int){
        Aarray_const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    template<typename E>
    typename Aarray<E>::Aarray_const_iterator& Aarray<E>::Aarray_const_iterator::operator--(){
        if(_index-1 <0){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        _index--;
        return *this;
    }

    template<typename E>
    typename Aarray<E>::Aarray_const_iterator Aarray<E>::Aarray_const_iterator::operator--(int){
        Aarray_const_iterator temp = *this;
        --(*this);
        return temp;
    }

    template<typename E>
    typename Aarray<E>::Aarray_const_iterator Aarray<E>::Aarray_const_iterator::operator+(int n) const{
        int new_index = _index + n;
        if(new_index <0 || new_index > _array->_length){
            throw Eexception("Exception: Index of out Aarray's range!");
        }
        return Aarray_const_iterator(new_index,_array);
    }
    
    template<typename E>
    typename Aarray<E>::Aarray_const_iterator Aarray<E>::Aarray_const_iterator::operator-(int n) const{
        return (*this) + (-n);
    }

    template<typename E>
    int Aarray<E>::Aarray_const_iterator::operator-(const Aarray_const_iterator& other) const{
        if(_array != other._array){
            throw Eexception("Exception: substract operator cannot be carried on different Aarray");
        }
        return _index - other._index;
    }

    template<typename E>
    bool Aarray<E>::Aarray_const_iterator::operator==(const Aarray_const_iterator& other) const{
        return _index == other._index && _array == other._array;
    }

    template<typename E>
    bool Aarray<E>::Aarray_const_iterator::operator!=(const Aarray_const_iterator& other) const{
        return !(*this == other);
    }
    
    template<typename E>
    typename Aarray<E>::Aarray_iterator Aarray<E>::begin() {
        if (this->empty()) {
            throw Eexception("Exception: Aarray is empty, cannot get begin()!");
        }
        return Aarray_iterator(0,this);
    }
    
    template<typename E>
    typename Aarray<E>::Aarray_iterator Aarray<E>::end(){
        return Aarray_iterator(this->_length,this);
    }

    template<typename E>
    typename Aarray<E>::Aarray_const_iterator Aarray<E>::begin() const{
        if(this->empty()){
            throw Eexception("Exception: Aarray is empty, cannot get begin()!");
        }
        return Aarray_const_iterator(0,this);
    }

    template<typename E>
    typename Aarray<E>::Aarray_const_iterator Aarray<E>::end() const{
        return Aarray_const_iterator(this->_length,this);
    }

    template<typename E>
    void Aarray<E>::fill(const E& value){
        for(Aarray_iterator it = begin();it!=end();it++){
            *it = value;
        }
    }
    
    template<typename E>
    void Aarray<E>::fill(const E& value,int start_index,int end_index){
        for(Aarray_iterator it = Aarray_iterator(start_index,this);it!=Aarray_iterator(end_index,this);it++){
            *it = value;
        }
    }

    template<typename E>
    int Aarray<E>::find_first(const E& value) const{
        for(Aarray_const_iterator it = begin();it!=end();it++){
            if(*it == value){
                return it - begin(); 
            }
        }
        return -1;
    }

    template<typename E>
    int Aarray<E>::find_first(const E& value,int start_index,int end_index) const{
        for(Aarray_const_iterator it = Aarray_const_iterator(start_index,this);it!=Aarray_const_iterator(end_index,this);it++){
            if(*it == value){
                return it - begin(); 
            }
        }
        return -1;
    }

    template<typename E>
    int Aarray<E>::find_last(const E& value,int start_index,int end_index) const{
        // 1. 范围合法性检查
        if (start_index < 0 || end_index > this->_length || start_index >= end_index) {
            return -1;  // 无效范围或空范围，直接返回-1
        }
        // 2. 从end_index-1开始反向遍历到start_index
        Aarray_const_iterator curr_it(end_index - 1, this);
        Aarray_const_iterator start_it(start_index, this);
        while (true) {
            if (*curr_it == value) {
                return curr_it - begin();  // 找到匹配值
            }
            if (curr_it == start_it) {
                break;  // 已遍历到起始位置，退出循环
            }
            --curr_it;  // 继续向前移动
        }
        // 3. 未找到匹配值
        return -1;
    }

    template<typename E>
    int Aarray<E>::find_last(const E& value){
        return find_last(value,0,this->_length);
    }
    
    template<typename E>
    bool Aarray<E>::contains(const E& value) const{
        return find_first(value)!=-1;
    }

    template<typename E>
    void Aarray<E>::swap(int index1,int index2){
        if(index1>=0&&index1<this->_length&&index2>=0&&index2<this->_length){
            if(index1!=index2){
                E tmp = this->_array[index1];
                this->_array[index1] = this->_array[index2];
                this->_array[index2] = tmp;
            }
            return;
        }
        throw Eexception("Exception: Index out of range or the indexs are the same");
    }

    template<typename E>
    void Aarray<E>::reverse(){
        for(int i=0,j=this->_length-1;i<j;i++,j--){
            swap(i,j);
        }
    }

    template<typename E>
    Aarray<E> Aarray<E>::subArr(int start_index,int end_index){
        if (start_index < 0 || end_index > this->_length || start_index >= end_index) {
            throw Eexception("Exception: Invalid subarray range (out of bounds or start >= end)!");
        }
    
        Aarray ans(end_index - start_index);  
        
        Aarray_const_iterator it = Aarray_const_iterator(start_index, this);
        Aarray_const_iterator end_it = Aarray_const_iterator(end_index, this);
        for (int i = 0; it != end_it; ++it, ++i) {
            ans.at(i) = *it;
        }

        return ans;
    }
}