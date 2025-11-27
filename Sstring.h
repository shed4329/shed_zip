#ifndef SSTRING_H
#define SSTRING_H

#include "Eexception.h"
#include "Vvector.h"
#include "Oostream.h"

namespace shed_std{
    constexpr int STRING_MAX_LENGTH = 65535;

    // --- 辅助函数声明 ---

    /**
     * @brief 内存拷贝，从源地址拷贝指定数量的字符到目标地址。
     * @param dest 目标字符数组的指针。
     * @param src 源字符数组的指针。
     * @param n 要拷贝的字符数量。
     */
    void memory_copy(char* dest,const char* src,int n);

    /**
     * @brief 内存填充，使用指定的字符填充目标地址的指定数量位置。
     * @param dest 目标字符数组的指针。
     * @param c 用于填充的字符。
     * @param n 要填充的字符数量。
     */
    void memory_set(char* dest,char c,int n);

    /**
     * @brief 返回C风格字符串的长度（遇到'\0'停止）。
     * @param str C风格字符串指针。
     * @return 字符串的长度，如果str为nullptr则返回0。
     */
    int string_length(const char* str);

    /**
     * @brief 对两个C风格字符串进行逐个字符比较。
     * @param a 第一个C风格字符串。
     * @param b 第二个C风格字符串。
     * @return 如果a > b，返回正数；如果a < b，返回负数；如果相同，返回长度差。
     */
    int string_compare(const char* a,const char* b);

    /**
     * @brief 返回C风格字符串在指定最大长度内的长度。
     * @param str C风格字符串指针。
     * @param n 最大检查长度限制。
     * @return 字符串的实际长度，不超过n。
     */
    int string_length_n(const char* str,int n = STRING_MAX_LENGTH);

    /**
     * @brief 在指定长度内比较两个C风格字符串。
     * @param a 第一个C风格字符串。
     * @param b 第二个C风格字符串。
     * @param n 最大比较长度限制。
     * @return 如果a > b，返回正数；如果a < b，返回负数；如果相同，返回长度差。
     */
    int string_compare_n(const char* a,const char* b,int n = STRING_MAX_LENGTH);

    // --- Sstring 类定义 ---

    class Sstring{
        private:
            char* _data; // 存储字符串数据
            int _size;   // 字符串长度
            int _capacity;//字符串容量

            // 容量扩充
            void _expand(int new_capacity);

            /**
             * 私有辅助函数:KMP算法 - 构建LPS数组
             * @param pattern 模式串的C风格指针
             * @param M 模式串长度
             * @return 包含LPS的Vvector<int>对象
             */
            Vvector<int> _compute_lps_array(const char* pattern,int M) const;
        public:
            /*
            * 默认构造函数:空字符串
            */ 
            Sstring();

            /*
            * 从C字符串构造
            */ 
            Sstring(const char* str);

            /**
             * 拷贝构造函数
             */
            Sstring(const Sstring& other);

            /**
             * 析构函数
             */
            ~Sstring();

            // 赋值运算符
            Sstring& operator=(const Sstring& other);

            /**
             * @brief 获取字符串的当前长度（包含的字符数）。
             * @return 字符串长度（int）。
             */
            int size() const;

            /**
             * @brief 获取字符串的当前长度（包含的字符数）。与size()功能相同。
             * @return 字符串长度（int）。
             */
            int length() const;

            /**
             * @brief 获取字符串的当前容量（已分配内存可以容纳的字符数）。
             * @return 字符串容量（int）。
             */
            int capacity() const;

            /**
             * @brief 检查字符串是否为空。
             * @return 如果字符串长度为0，返回true；否则返回false。
             */
            bool empty() const;

            /**
             * @brief 字符访问：返回指定索引位置的字符引用，带下标检查。
             * @param index 要访问的字符索引。
             * @return 索引位置的字符引用。
             * @throw Eexception 当索引超出范围[0, _size-1]时抛出异常。
             */
            char& at(int index);

             /**
             * @brief 字符访问：返回指定索引位置的字符常量引用，带下标检查。
             * @param index 要访问的字符索引。
             * @return 索引位置的字符常量引用。
             * @throw Eexception 当索引超出范围[0, _size-1]时抛出异常。
             */
            const char& at(int index) const;

            // []下标访问元素
            char& operator[](int index);

            // []下标访问元素，供常量使用
            const char& operator[](int index) const;

            /**
             * @brief 返回字符串的第一个元素。
             * @return 第一个元素的引用。
             */
            char& front();

            /**
             * @brief 返回字符串的第一个元素（常量）。
             * @return 第一个元素的常量引用。
             */
            const char& front() const;

            /**
             * @brief 返回字符串的最后一个元素。
             * @return 最后一个元素的引用。
             */
            char& back();

            /**
             * @brief 返回字符串的最后一个元素（常量）。
             * @return 最后一个元素的常量引用。
             */
            const char& back() const;

            /**
             * @brief 返回C语言风格的字符串（空终止）。
             * @return C风格字符串的常量指针（const char*）。
             */
            const char* c_string() const;

            /**
             * @brief 在指定位置插入一个字符。
             * @param index 插入位置的索引（0 <= index <= _size）。
             * @param c 要插入的字符。
             * @throw Eexception 当插入位置不合法或超出最大长度限制时抛出异常。
             */
            void insert(int index,char c);

            /**
             * @brief 在指定位置插入一个Sstring。
             * @param index 插入位置的索引（0 <= index <= _size）。
             * @param str 要插入的Sstring对象。
             * @throw Eexception 当插入位置不合法或超出最大长度限制时抛出异常。
             */
            void insert(int index,const Sstring& str);

            /**
             * @brief 抹除指定索引位置的单个字符。
             * @param index 要删除的字符索引（0 <= index < _size）。
             * @throw Eexception 当索引不合法时抛出异常。
             */
            void erease(int index);

            /**
             * @brief 替换指定位置的单个字符。
             * @param index 要替换的字符索引（0 <= index < _size）。
             * @param c 新的字符。
             * @throw Eexception 当索引不合法时抛出异常。
             */
            void replace(int index,char c);

            /**
             * @brief 将指定索引位置的单个字符替换为另一个Sstring。
             * @param index 要替换的字符索引（0 <= index < _size）。
             * @param other 用于替换的Sstring对象。
             * @throw Eexception 当索引不合法或替换后超出最大长度限制时抛出异常。
             */
            void replace(int index,const Sstring& other);

            // += 拼接字符串
            Sstring& operator+=(const Sstring& other);

            // += 拼接字符串
            Sstring& operator+=(const char* str);

            /**
             * @brief 在字符串末尾添加一个字符。
             * @param c 要添加的字符。
             * @throw Eexception 当超出最大长度限制时抛出异常。
             */
            void push_back(char c);

            /**
             * @brief 预留足够的容量，确保内存可以容纳至少size个字符。
             * @param size 期望的最小容量。
             * @throw Eexception 当size超出最大长度限制时抛出异常。
             */
            void reserve(int size);

            /**
             * @brief 调整字符串的长度。如果新长度大于当前长度，会进行填充；如果小于，则会截断。
             * @param new_size 期望的新长度。
             * @param fill_char 填充时使用的字符（默认为'\0'）。
             * @throw Eexception 当new_size为负数或超出最大长度限制时抛出异常。
             */
            void resize(int new_size,char fill_char='\0');
            
            /**
             * @brief 缩短容量，使其与当前字符串长度(_size)一致，以释放多余内存。
             */
            void shrink_to_fit();

            /**
             * @brief 清空字符串内容，将长度设为0，但不改变容量。
             */
            void clear();

            // 字符串比较
            bool operator==(const Sstring& other) const;

            bool operator!=(const Sstring& other) const;

            bool operator<(const Sstring& other) const;

            bool operator<=(const Sstring& other) const;

            bool operator>(const Sstring& other) const;

            bool operator>=(const Sstring& other) const;

            // --- 迭代器定义 (为了简洁，保持内嵌实现) ---
            
            class Sstring_iterator {
                public:
                    // 迭代器所需类型定义
                    using value_type = char;
                    using difference_type = int;
                    using pointer = char*;
                    using reference = char&;

                    // 构造函数
                    explicit Sstring_iterator(pointer p) : current(p) {}

                    // 解引用运算符
                    reference operator*() const {
                        return *current;
                    }

                    pointer operator->() const {
                        return current;
                    }

                    // 前置递增/递减 (Prefix)
                    Sstring_iterator& operator++() {
                        ++current;
                        return *this;
                    }

                    Sstring_iterator& operator--() {
                        --current;
                        return *this;
                    }

                    // 后置递增/递减 (Postfix)
                    Sstring_iterator operator++(int) {
                        Sstring_iterator temp = *this;
                        ++current;
                        return temp;
                    }

                    Sstring_iterator operator--(int) {
                        Sstring_iterator temp = *this;
                        --current;
                        return temp;
                    }

                    // 比较运算符
                    bool operator==(const Sstring_iterator& other) const {
                        return current == other.current;
                    }

                    bool operator!=(const Sstring_iterator& other) const {
                        return current != other.current;
                    }

                    // 随机访问运算符
                    Sstring_iterator operator+(difference_type n) const {
                        return Sstring_iterator(current + n);
                    }

                    Sstring_iterator operator-(difference_type n) const {
                        return Sstring_iterator(current - n);
                    }

                    difference_type operator-(const Sstring_iterator& other) const {
                        return current - other.current;
                    }

                    Sstring_iterator& operator+=(difference_type n) {
                        current += n;
                        return *this;
                    }

                    Sstring_iterator& operator-=(difference_type n) {
                        current -= n;
                        return *this;
                    }

                    reference operator[](difference_type n) const {
                        return current[n];
                    }

                private:
                    pointer current;
                };
            
            class Sstring_const_iterator {
                public:
                    // 迭代器所需类型定义
                    using value_type = char;
                    using difference_type = int;
                    using pointer = const char*;
                    using reference = const char&;

                    // 构造函数
                    explicit Sstring_const_iterator(pointer p) : current(p) {}

                    // 解引用运算符
                    reference operator*() const {
                        return *current;
                    }

                    pointer operator->() const {
                        return current;
                    }

                    // 前置递增/递减 (Prefix)
                    Sstring_const_iterator& operator++() {
                        ++current;
                        return *this;
                    }

                    Sstring_const_iterator& operator--() {
                        --current;
                        return *this;
                    }

                    // 后置递增/递减 (Postfix)
                    Sstring_const_iterator operator++(int) {
                        Sstring_const_iterator temp = *this;
                        ++current;
                        return temp;
                    }

                    Sstring_const_iterator operator--(int) {
                        Sstring_const_iterator temp = *this;
                        --current;
                        return temp;
                    }

                    // 比较运算符
                    bool operator==(const Sstring_const_iterator& other) const {
                        return current == other.current;
                    }

                    bool operator!=(const Sstring_const_iterator& other) const {
                        return current != other.current;
                    }

                    // 随机访问运算符
                    Sstring_const_iterator operator+(difference_type n) const {
                        return Sstring_const_iterator(current + n);
                    }

                    Sstring_const_iterator operator-(difference_type n) const {
                        return Sstring_const_iterator(current - n);
                    }

                    difference_type operator-(const Sstring_const_iterator& other) const {
                        return current - other.current;
                    }

                    Sstring_const_iterator& operator+=(difference_type n) {
                        current += n;
                        return *this;
                    }

                    Sstring_const_iterator& operator-=(difference_type n) {
                        current -= n;
                        return *this;
                    }

                    reference operator[](difference_type n) const {
                        return current[n];
                    }

                private:
                    pointer current;
                };

            /**
             * @brief 返回指向字符串起始字符的正向迭代器。
             * @return Sstring_iterator 指向第一个字符。
             */
            Sstring_iterator begin();

            /**
             * @brief 返回指向字符串末尾之后位置的正向迭代器。
             * @return Sstring_iterator 指向空字符'\0'的位置。
             */
            Sstring_iterator end(); 

            /**
             * @brief 返回指向字符串起始字符的常量正向迭代器。
             * @return Sstring_const_iterator 指向第一个字符。
             */
            Sstring_const_iterator begin() const;

            /**
             * @brief 返回指向字符串末尾之后位置的常量正向迭代器。
             * @return Sstring_const_iterator 指向空字符'\0'的位置。
             */
            Sstring_const_iterator end() const;
            
            // cbegin / cend 
            /**
             * @brief 返回指向字符串起始字符的常量正向迭代器。
             * @return Sstring_const_iterator 指向第一个字符。
             */
            Sstring_const_iterator cbegin() const;

            /**
             * @brief 返回指向字符串末尾之后位置的常量正向迭代器。
             * @return Sstring_const_iterator 指向空字符'\0'的位置。
             */
            Sstring_const_iterator cend() const;

            // --- 反向迭代器接口 ---

            /**
             * @brief 返回指向字符串最后一个字符的反向迭代器。
             * @return Sstring_iterator 指向最后一个字符。
             */
            Sstring_iterator rbegin();

            /**
             * @brief 返回指向字符串第一个字符前一个位置的反向迭代器（终止位置）。
             * @return Sstring_iterator 指向第一个字符的前一个位置。
             */
            Sstring_iterator rend(); 
            
            /**
             * @brief 返回指向字符串最后一个字符的常量反向迭代器。
             * @return Sstring_const_iterator 指向最后一个字符。
             */
            Sstring_const_iterator rbegin() const;

            /**
             * @brief 返回指向字符串第一个字符前一个位置的常量反向迭代器（终止位置）。
             * @return Sstring_const_iterator 指向第一个字符的前一个位置。
             */
            Sstring_const_iterator rend() const;

            /**
             * @brief 返回指向字符串最后一个字符的常量反向迭代器。
             * @return Sstring_const_iterator 指向最后一个字符。
             */
            Sstring_const_iterator crbegin() const;

            /**
             * @brief 返回指向字符串第一个字符前一个位置的常量反向迭代器（终止位置）。
             * @return Sstring_const_iterator 指向第一个字符的前一个位置。
             */
            Sstring_const_iterator crend() const;


            /**
             * @brief 查找子串（基于KMP算法），返回第一次出现的位置。
             * @param pattern 需要查找的子串。
             * @param pos 开始查找的位置（可选，默认为0）。
             * @return 找到的第一个匹配位置的索引，如果没有找到，返回-1。
             */
            int find(const Sstring& pattern,int pos = 0) const;

            /**
             * @brief 查找C风格子串（基于KMP算法），返回第一次出现的位置。
             * @param str 需要查找的C风格子串。
             * @param pos 开始查找的位置（可选，默认为0）。
             * @return 找到的第一个匹配位置的索引，如果没有找到，返回-1。
             */
            int find(const char* str,int pos = 0) const;

            /**
             * @brief 检查字符串是否包含指定的子串。
             * @param str 要查找的子串。
             * @return 如果包含，返回true；否则返回false。
             */
            bool contains(const Sstring& str) const;

            /**
             * @brief 检查字符串是否包含指定的C风格子串。
             * @param str 要查找的C风格子串。
             * @return 如果包含，返回true；否则返回false。
             */
            bool contains(const char* str) const;

            /**
             * @brief 提取子串。
             * @param start 子串的起始索引（包含）。
             * @param end 子串的结束索引（不包含）。
             * @return 包含指定范围元素的新Sstring对象。
             * @throw Eexception 当范围不合法（start<0 || end>=_size || end<start）时抛出异常。
             */
            Sstring substr(int start,int end) const;
            
            /**
             * @brief 拷贝字符串的子序列到外部缓冲区。
             * @param dest 目标字符数组的指针。
             * @param count 要复制的最大字符数。
             * @param pos 起始复制的索引位置 (可选，默认从 0 开始)。
             * @return 实际复制的字符数。
             * @note 拷贝的字符串末尾不包含'\0'终止符。
             * @throw Eexception 如果目标缓冲区为空、起始位置或复制长度不合法时抛出异常。
             */
            int copy(char* dest, int count, int pos = 0) const;

    };

    // --- 非成员运算符重载声明 ---
    
    // 加法支持
    inline Sstring operator+(const Sstring& lhs, const Sstring& rhs);

    inline Sstring operator+(const Sstring& lhs, const char* rhs);

    inline Sstring operator+(const Sstring& lhs, char rhs);

    inline Sstring operator+(const char* lhs, const Sstring& rhs);

    inline Sstring operator+(char lhs, const Sstring& rhs);
    
    // 支持使用输出流输出
    inline Oostream& operator<<(Oostream& output_stream,const Sstring& str);

}

#include "Sstring.tpp"

#endif // SSTRING_H