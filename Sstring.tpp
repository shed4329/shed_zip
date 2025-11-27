#ifndef SSTRING_TPP
#define SSTRING_TPP

#include "Sstring.h" // 包含头文件，提供所有声明

namespace shed_std{
    // --- 辅助函数实现 ---

    void memory_copy(char* dest,const char* src,int n){
        for(int i = 0;i<n;i++){
            dest[i] = src[i];
        }
    }

    void memory_set(char* dest,char c,int n){
        for(int i=0;i<n;i++){
            dest[i] = c;
        }
    }

    int string_length(const char* str){
        if(str == nullptr) return 0;
        int len = 0;
        while(str[len]!='\0'){
            ++len;
        }
        return len;
    }

    int string_compare(const char* a,const char* b){
        int len_a = string_length(a);
        int len_b = string_length(b);
        int min_len = (len_a<len_b)?len_a:len_b;
        for(int i = 0;i<min_len;i++){
            if(a[i]!=b[i]){
                return a[i]-b[i];
            }
        }
        return len_a - len_b;//完全相同的时候，长度短的小
    }

    int string_length_n(const char* str,int n){
        if(str == nullptr) return 0;
        int len = 0;
        for(int i=0;i<n;++i){
            if(str[i] == '\0') break;
            ++len;
        }
        return len;
    }

    int string_compare_n(const char* a,const char* b,int n){
        int len_a = string_length_n(a,n);
        int len_b = string_length_n(b,n);
        int min_len = (len_a<len_b)?len_a:len_b;
        for(int i = 0;i<min_len;i++){
            if(a[i]!=b[i]){
                return a[i]-b[i];
            }
        }
        return len_a - len_b;//完全相同的时候，长度短的小
    }

    // --- Sstring 类成员函数实现 ---

    void Sstring::_expand(int new_capacity){
        if(new_capacity < _capacity) return;

        // 检查是否超出最大长度限制
        if(new_capacity > STRING_MAX_LENGTH){
            throw Eexception("Sstring:exceed max length!");
        }

        if(new_capacity < 0){
            throw Eexception("Sstring:Illegal length");
        }

        // 分配内存并拷贝数据
        char* new_data = new char[new_capacity+1];
        memory_copy(new_data,_data,_size);
        new_data[_size] = '\0';
        // 删除原来的字符串，并更新
        delete[] _data;
        _data = new_data;
        _capacity = new_capacity;
    }

    Vvector<int> Sstring::_compute_lps_array(const char* pattern,int M) const{
        Vvector<int> lps;

        if(M<=0){
            return lps;// 返回空Vvector
        }

        // 填充M个0
        for(int i=0;i<M;++i){
            lps.push_back(0);
        }

        int len = 0; // 当前LPS长度
        int i = 1;  // 模式串的索引

        while(i<M){
            if(pattern[i] == pattern[len]){
                // 匹配成功
                len++;
                lps[i] = len;
                i++;
            }else{
                // 匹配失败
                if(len!=0){
                    len = lps[len-1];//len回退
                }else{
                    lps[i] = 0;//无法回退，len=0
                    i++;
                }
            }
        }
        return lps;
    }
        
    Sstring::Sstring():_data(new char[1]),_size(0),_capacity(0){
        _data[0] = '\0';
    }

    Sstring::Sstring(const char* str){
        if(str==nullptr){
            throw Eexception("Sstring: could not construct from null pointer!");
        }
        
        // 初始化
        _size = string_length_n(str);
        _capacity = _size;
        _data = new char[_capacity+1];
        memory_copy(_data,str,_size);
        _data[_size] = '\0';
    }

    Sstring::Sstring(const Sstring& other):_size(other._size),_capacity(other._capacity){
        // 拷贝
        _data = new char[_capacity + 1];
        memory_copy(_data,other._data,_size);
        _data[_size] = '\0';
    }

    Sstring::~Sstring(){
        delete[] _data;
    }

    Sstring& Sstring::operator=(const Sstring& other){
        if(this != &other){
            // 删除当前数据
            delete[] _data;
            // 经典拷贝操作
            _size = other._size;
            _capacity = other._capacity;
            _data = new char[_capacity + 1];
            memory_copy(_data,other._data,_size);
            _data[_size] = '\0';
        }
        return *this;
    }

    int Sstring::size() const{
        return _size;
    }

    int Sstring::length() const{
        return _size;
    }

    int Sstring::capacity() const{
        return _capacity;
    }

    bool Sstring::empty() const{
        return _size == 0;
    }

    char& Sstring::at(int index){
        if(index<0||index >= _size){
            throw Eexception("Sstring: Index out of range!");
        }
        return _data[index];
    }

    const char& Sstring::at(int index) const{
        if(index<0||index >= _size){
            throw Eexception("Sstring: Index out of range!");
        }
        return _data[index];
    }

    char& Sstring::operator[](int index){
        return at(index);
    }

    const char& Sstring::operator[](int index) const{
        return at(index);
    }

    char& Sstring::front(){
        return at(0);
    }

    const char& Sstring::front() const{
        return at(0);
    }

    char& Sstring::back(){
        return at(_size-1);
    }

    const char& Sstring::back() const{
        return at(_size-1);
    }

    const char* Sstring::c_string() const{
        return _data;
    }

    void Sstring::insert(int index,char c){
         // 检查插入位置合法性（pos可等于size()，表示插入到末尾）
        if (index < 0 || index > _size) {
            throw Eexception("Sstring insert: invalid position");
        }
        // 检查是否超出最大长度
        if (_size + 1 > STRING_MAX_LENGTH) {
            throw Eexception("Sstring insert: exceed max length");
        }

        // 容量不足时扩容
        if (_size + 1 > _capacity) {
            _expand(_size + 1);
        }   
        
        // 将index及其之后的字符向后移动1位
        for(int i = _size;i>index;--i){
            _data[i] = _data[i-1];
        }
        // 插入字符串
        _data[index] = c;
        // 更新长度和截止符
        _size++;
        _data[_size] = '\0';
    }

    void Sstring::insert(int index,const Sstring& str){
         // 检查插入位置合法性（pos可等于size()，表示插入到末尾）
        if (index < 0 || index > _size) {
            throw Eexception("Sstring insert: invalid position");
        }
        int len = str._size;
        // 检查是否超出最大长度
        if(_size + len > STRING_MAX_LENGTH){
            throw Eexception("Sstring insert: exceed max length");
        }
        // 分配空间
        if(_size+len>_capacity){
            _expand(_size+len);
        }

        // 将index及其之后的元素移动len位
        for(int i = _size +len-1;i>index+len-1;--i){
            _data[i] = _data[i-len];
        }
        // 拷贝
        memory_copy(_data+index,str._data,len);
        _size += len;
        _data[_size] = '\0';
    }

    void Sstring::erease(int index){
        // 检查插入位置合法性（pos可等于size()，表示插入到末尾）
        if (index < 0 || index >= _size) {
            throw Eexception("Sstring erease: invalid position");
        }
        // 将后面的数据拷贝到前面来,不用管终止符
        for(int i = index;i<_size - 1;i++){
            _data[i] = _data[i+1];
        }
        _size--;
        _data[_size] = '\0';
    }

    void Sstring::replace(int index,char c){
        // 检查插入位置合法性（pos可等于size()，表示插入到末尾）
        if (index < 0 || index >= _size) {
            throw Eexception("Sstring replace: invalid position");
        }
        _data[index] = c;
    }

    void Sstring::replace(int index,const Sstring& other){
         if (index < 0 || index >= _size) {
            throw Eexception("Sstring replace: invalid position");
        }
        if(other._size == 0) erease(index); // 等价于杀出
        else if(other._size == 1) replace(index,other.front());
        else{
            // 有点长的情况
            // 4. 替换为长字符串（len ≥ 2）
            int len = other._size;
            // 计算替换后的总长度：原长度 - 1（删除的字符） + len（新增的字符）
            int new_size = _size - 1 + len;
            // 检查是否超出最大长度限制
            if (new_size > STRING_MAX_LENGTH) {
                throw Eexception("Sstring replace: exceed max length"); // 修正提示文案
            }

            // 5. 容量不足则扩容（目标容量为new_size）
            if (new_size > _capacity) {
                _expand(new_size);
            }

            // 6. 先删除index位置的1个原字符（关键：实现“替换”而非“插入”）
            erease(index);

            // 7. 移动字符：为插入的len个字符腾出空间（从后往前移，避免覆盖）
            // 起始位置：原字符串最后一个字符的新位置（_size - 1 + len - 1，因已删除1个字符，_size已减1）
            // 终止位置：index + len（插入字符串的末尾后一位）
           for (int i = _size + len - 1; i -len >= index; --i) {
                _data[i] = _data[i - len]; 
            }

            // 8. 拷贝新字符串到index位置
            memory_copy(_data + index, other._data, len);

            // 9. 更新长度和终止符（_size已因erease减1，此处只需加len）
            _size += len;
            _data[_size] = '\0';
        }
    }

    Sstring& Sstring::operator+=(const Sstring& other){
        int new_size = _size + other._size;
        if(new_size > STRING_MAX_LENGTH){
            throw Eexception("Sstring: concat exceed max length!");
        }

        // 容量不够的时候填充
        if(new_size > _capacity){
            _expand(new_size);
        }
        // 拷贝other字符串到末端
        memory_copy(_data+ _size,other._data,other._size );
        _size = new_size;
        _data[_size] = '\0';
        return *this;
    }

    Sstring& Sstring::operator+=(const char* str){
        if (str == nullptr) {
            throw Eexception("Sstring: concat with null pointer");
        }
        Sstring temp(str);
        *this += temp;
        return *this;
    }

    void Sstring::push_back(char c){
        if(_size+1>STRING_MAX_LENGTH){
            throw Eexception("Sstring: push_back exceed max length");
        }

        // 容量足够
        if(_size+1>_capacity){
            _expand(_size+1);
        }

        _data[_size] =c;//追加字符
        _size++;
        _data[_size] = '\0';
    }

    void Sstring::reserve(int size){
        _expand(size);//expand有是否需要扩容的检查，这里什么都不用干
    }

    void Sstring::resize(int new_size,char fill_char){
        if(new_size <0){
            throw Eexception("Sstring: resize with negative size");
        }

        if(new_size > STRING_MAX_LENGTH){
            throw Eexception("Sstring: resize exceed max length");
        }

        if(new_size > _size){
            // 填充
            reserve(new_size);
            memory_set(_data+_size,fill_char,new_size-_size);
        }

        _size = new_size;
        _data[_size] = '\0';// 更新停止符
    }
    
    void Sstring::shrink_to_fit(){
        if(_capacity == _size) return;//什么都不用做
        // 拷贝一下
        char* new_data = new char[_size+1];
        memory_copy(new_data,_data,_size);
        new_data[_size] = '\0';
        
        // 释放内存并更新
        delete[] _data;
        _data = new_data;
        _capacity = _size;
    }

    void Sstring::clear(){
        _size = 0;
        _data[0] = '\0';
    }

    bool Sstring::operator==(const Sstring& other) const{
        if(_size != other._size) return false;
        return string_compare_n(_data,other._data,_size) == 0;
    }

    bool Sstring::operator!=(const Sstring& other) const{
        return !(*this == other);
    }

    bool Sstring::operator<(const Sstring& other) const{
        return string_compare_n(_data,other._data) <0;
    }

    bool Sstring::operator<=(const Sstring& other) const{
        return string_compare_n(_data,other._data) <=0;
    }

    bool Sstring::operator>(const Sstring& other) const{
        return string_compare_n(_data,other._data) >0;
    }

    bool Sstring::operator>=(const Sstring& other) const{
        return string_compare_n(_data,other._data) >=0;
    }
    
    // --- 迭代器实现 (仅链接到内联定义) ---

    Sstring::Sstring_iterator Sstring::begin() {
        return Sstring_iterator(_data);
    }

    Sstring::Sstring_iterator Sstring::end() {
        return Sstring_iterator(_data + _size); 
    }

    Sstring::Sstring_const_iterator Sstring::begin() const {
        return Sstring_const_iterator(_data);
    }

    Sstring::Sstring_const_iterator Sstring::end() const {
        return Sstring_const_iterator(_data + _size);
    }
    
    Sstring::Sstring_const_iterator Sstring::cbegin() const {
        return begin();
    }

    Sstring::Sstring_const_iterator Sstring::cend() const {
        return end();
    }

    Sstring::Sstring_iterator Sstring::rbegin() {
        // end()指向'\0'，--end()指向最后一个字符
        return Sstring_iterator(end().operator-=(1)); 
    }

    Sstring::Sstring_iterator Sstring::rend() {
        // begin()指向第一个字符，--begin()指向第一个字符前一位
        return Sstring_iterator(begin().operator-=(1)); 
    }
    
    Sstring::Sstring_const_iterator Sstring::rbegin() const {
        return Sstring_const_iterator(end().operator-=(1));
    }

    Sstring::Sstring_const_iterator Sstring::rend() const {
        return Sstring_const_iterator(begin().operator-=(1));
    }

    Sstring::Sstring_const_iterator Sstring::crbegin() const {
        return rbegin();
    }

    Sstring::Sstring_const_iterator Sstring::crend() const {
        return rend();
    }


    int Sstring::find(const Sstring& pattern,int pos) const{
        // 边界检查
        if(pattern._size == 0 ) return (pos >= 0 && pos <= _size) ? pos : -1;
        if(pos <0 || pos >= _size + 1) return -1;

        int N = _size; // 字符串长度
        int M = pattern._size;//模式串长度

        if(N - M < pos) return -1;

        // 1.构建LPS数组
        const char* pat_data = pattern.c_string();
        Vvector<int> lps = _compute_lps_array(pat_data,M);

        // 2.KMP 查找匹配
        int i = pos; // 字符串的索引
        int j = 0;   // 模式串的索引
        
        while(i<N){
            if(pat_data[j] == _data[i]){
                // 匹配成功，去下一个字符
                i++;
                j++;
                if( j == M){
                    //完全匹配时
                    int found_index = i-j;
                    return found_index;
                }
            }else if( i<N && pat_data[j] != _data[i]){
                // 匹配失败
                if(j!=0){
                    // 根据LPS回退
                    j = lps[j-1];
                }else{
                    // j == 0，无法回退
                    i++;
                }
            }
        }
        // 没有找到
        return -1;
    }

    int Sstring::find(const char* str,int pos) const{
        if (str == nullptr) {
            return -1; 
        }
        Sstring temp(str);
        return find(temp, pos);
    }

    bool Sstring::contains(const Sstring& str) const{
        return find(str)!=-1;
    }

    bool Sstring::contains(const char* str) const{
        return find(str)!=-1;
    }

    Sstring Sstring::substr(int start,int end) const{
        if(start<0||end>=_size||end<start){
            throw Eexception("Illegal index");
        }

        // 1.计算子串长度
        int len = end-start;
        // 长度为0时返回空串
        if (len == 0) {
            return Sstring();
        }
        // 2.分配给C-style string
        char* temp_data = new char[len+1];
        // 3.拷贝子串
        memory_copy(temp_data,_data + start,len);
        // 4.添加空终止符
        temp_data[len] = '\0';
        // 5.创建Sstring对象
        Sstring result(temp_data);
        // 6.释放内存
        delete[] temp_data;
        // 7.返回结果
        return result;
    }
    
    int Sstring::copy(char* dest, int count, int pos) const {
        // 1. 边界和空指针检查
        if (dest == nullptr) {
            // 目标缓冲区为空，无法复制
            throw Eexception("Sstring copy: Destination buffer is null.");
        }
        if (pos < 0 || pos > _size) {
            // 起始位置必须在 [0, _size] 范围内 (pos == _size 表示复制长度为 0)
            throw Eexception("Sstring copy: Illegal starting position.");
        }
        if (count < 0) {
            // 复制长度不能为负
            throw Eexception("Sstring copy: Copy count cannot be negative.");
        }

        // 2.需要复制的字符
        int chars_left = _size - pos;
        int actual_count = (count < chars_left) ? count : chars_left; // 取两者的较小值

        if(actual_count == 0){
            return 0;
        }

        // 3.内存复制
        memory_copy(dest,_data+pos,actual_count);
        
        return actual_count;
    }
    
    // --- 非成员运算符重载实现 (保持内联) ---

    inline Sstring operator+(const Sstring& lhs, const Sstring& rhs) {
        Sstring result = lhs;  // 拷贝 lhs
        result += rhs;         // 复用 += 运算符
        return result;
    }

    inline Sstring operator+(const Sstring& lhs, const char* rhs) {
        Sstring temp(rhs);
        return lhs + temp;
    }

    inline Sstring operator+(const Sstring& lhs, char rhs) {
        Sstring result = lhs;
        result.push_back(rhs);
        return result;
    }

    inline Sstring operator+(const char* lhs, const Sstring& rhs) {
        Sstring temp(lhs);
        return temp + rhs;
    }

    inline Sstring operator+(char lhs, const Sstring& rhs) {
        char temp_str[2];
        temp_str[0] = lhs;
        temp_str[1] = '\0';
        Sstring temp(temp_str);
        return temp + rhs;
    }
    
    // 支持使用输出流输出
    inline Oostream& operator<<(Oostream& output_stream,const Sstring& str){
        output_stream << str.c_string();
        return output_stream;
    }
    
}

#endif // SSTRING_TPP