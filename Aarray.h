#ifndef AARRAY_H
#define AARRAY_H

#include "Eexception.h"

namespace shed_std{
    template <typename T>
    class basic_array{
        protected:
            T* _array; // 数组
            int _length;//长度
        public:
            /**
             * 构造函数，初始化指定长度的数组
             * @param N 数组长度，必须>=0
             */
            basic_array(int N);

            /**
             * 析构函数，释放数组内存
             */
            ~basic_array();

            /**
             * 访问指定下标的元素，无检查环节
             * @param index 下标
             * @return 元素的引用
             */
            T& operator[] (int index);
            
            /**
             * 访问指定下标的元素，无检查环节，供常量使用
             * @param index 下标
             * @return 元素的常量引用
             */
            const T& operator[](int index) const;
            
            /**
             * 访问指定下标的元素，提供下标检查
             * @param index 下标
             * @param return_value 用于存储返回值的引用
             * @return 成功获取元素返回true，下标越界返回false
             */
            bool at(int index, T& return_value);

            /**
             * 访问指定下标的元素，提供下标检查,供常量使用
             * @param index 下标
             * @param return_value 用于存储返回值的引用
             * @return 成功获取元素返回true，下标越界返回false
             */
            bool at(int index, T& return_value) const;

            /**
             * 返回第一个元素
             * @return 第一个元素的引用
             * @note 当数组为空时访问会导致未定义行为
             */
            T& front();

            /**
             * 返回第一个元素，供常量使用
             * @return 第一个元素的常量引用
             * @note 当数组为空时访问会导致未定义行为
             */
            const T& front() const;

            /**
             * 返回最后一个元素
             * @return 最后一个元素的引用
             * @note 当数组为空时访问会导致未定义行为
             */
            T& back();

            /**
             * 返回最后一个元素
             * @return 最后一个元素的常量引用
             * @note 当数组为空时访问会导致未定义行为
             */
            const T& back() const;

            /**
             * 返回数组长度
             * @return 数组当前的长度
             */
            const int length() const;

            /**
             * 判断数组是否为空
             * @return 数组为空返回true，否则返回false
             */
            bool empty() const;
    };

    template<typename E>
    class Aarray:public basic_array<E>{
        private:

        public:
            /**
             * 构造函数，初始化指定长度的数组
             * @param N 数组长度，必须>=0
             * @throw Eexception 当N<0时抛出异常
             */
            Aarray(int N);
            
            /**
             * 拷贝构造函数
             * @param other 另外一个Aarray
             */
            Aarray(const Aarray& other);

            /**
             * 赋值运算符重载
             * @param other 另外一个Aarray
             * @return 当前对象的引用（支持连续赋值）
             */
            Aarray& operator= (const Aarray& other);
            
            /**
             * 返回某个下标对应的值，提供下标检查
             * @param index 下标
             * @return 元素的引用
             * @throw Eexception 当下标越界时抛出异常
             */
            E& at(int index);
            
            /**
             * 返回某个下标对应的值，提供下标检查，供常量使用
             * @param index 下标
             * @return 元素的常量引用
             * @throw Eexception 当下标越界时抛出异常
             */
            const E& at(int index) const;
            
            /**
             * 返回第一个元素的值，提供下标检查
             * @return 第一个元素的引用
             * @throw Eexception 当数组为空时抛出异常
             */
            E& front();

            /**
             * 返回第一个元素的值，提供下标检查，供常量使用
             * @return 第一个元素的常量引用
             * @throw Eexception 当数组为空时抛出异常
             */
            const E& front() const;
            
            /**
             * 返回最后元素的值，提供下标检查
             * @return 最后一个元素的引用
             * @throw Eexception 当数组为空时抛出异常
             */
            E& back();

            /**
             * 返回最后元素的值，提供下标检查，供常量使用
             * @return 最后一个元素的常量引用
             * @throw Eexception 当数组为空时抛出异常
             */
            const E& back() const;

            /**
             * 普通迭代器类，用于遍历和修改数组元素
             */
            class Aarray_iterator{
                private:
                    int _index;       // 当前迭代器指向的下标
                    Aarray* _array;   // 指向所属的数组对象
                public:
                    /**
                     * 迭代器构造函数
                     * @param index 初始下标位置
                     * @param array 所属的数组指针
                     * @throw Eexception 当下标越界时抛出异常
                     */
                    Aarray_iterator(int index,Aarray* array);

                    /**
                     * 解引用操作符，获取当前元素的引用
                     * @return 当前元素的引用
                     */
                    E& operator*() const;

                    /**
                     * 成员访问操作符，获取当前元素的指针
                     * @return 当前元素的指针
                     */
                    E* operator->() const;

                    /**
                     * 前置递增运算符，移动到下一个元素
                     * @return 更新后的迭代器引用
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_iterator& operator++();

                    /**
                     * 后置递增运算符，移动到下一个元素
                     * @return 递增前的迭代器副本
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_iterator operator++(int);

                    /**
                     * 前置递减运算符，移动到上一个元素
                     * @return 更新后的迭代器引用
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_iterator& operator--();

                    /**
                     * 后置递减运算符，移动到上一个元素
                     * @return 递减前的迭代器副本
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_iterator operator--(int);

                    /**
                     * 迭代器加法，移动指定步数
                     * @param n 要移动的步数（可正可负）
                     * @return 移动后的新迭代器
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_iterator operator+(int n) const;
                    
                    /**
                     * 迭代器减法，移动指定步数（等价于加负数）
                     * @param n 要移动的步数
                     * @return 移动后的新迭代器
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_iterator operator-(int n) const;

                    /**
                     * 迭代器差值计算，获取两个迭代器之间的距离
                     * @param other 另一个迭代器
                     * @return 两个迭代器的下标差值
                     * @throw Eexception 当两个迭代器不属于同一个数组时抛出异常
                     */
                    int operator-(const Aarray_iterator& other) const;

                    /**
                     * 相等比较运算符
                     * @param other 另一个迭代器
                     * @return 两个迭代器指向同一位置且属于同一数组时返回true
                     */
                    bool operator==(const Aarray_iterator& other) const;

                    /**
                     * 不等比较运算符
                     * @param other 另一个迭代器
                     * @return 两个迭代器不指向同一位置或不属于同一数组时返回true
                     */
                    bool operator!=(const Aarray_iterator& other) const;
            };

            /**
             * 常量迭代器类，用于遍历数组元素（不可修改）
             */
            class Aarray_const_iterator{
                private:
                    int _index;           // 当前迭代器指向的下标
                    const Aarray* _array; // 指向所属的常量数组对象
                public:
                    /**
                     * 常量迭代器构造函数
                     * @param index 初始下标位置
                     * @param array 所属的常量数组指针
                     * @throw Eexception 当下标越界时抛出异常
                     */
                    Aarray_const_iterator(int index,const Aarray* array);

                    /**
                     * 解引用操作符，获取当前元素的常量引用
                     * @return 当前元素的常量引用
                     */
                    const E& operator*() const;

                    /**
                     * 成员访问操作符，获取当前元素的常量指针
                     * @return 当前元素的常量指针
                     */
                    const E* operator->() const;

                    /**
                     * 前置递增运算符，移动到下一个元素
                     * @return 更新后的迭代器引用
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_const_iterator& operator++();

                    /**
                     * 后置递增运算符，移动到下一个元素
                     * @return 递增前的迭代器副本
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_const_iterator operator++(int);

                    /**
                     * 前置递减运算符，移动到上一个元素
                     * @return 更新后的迭代器引用
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_const_iterator& operator--();

                    /**
                     * 后置递减运算符，移动到上一个元素
                     * @return 递减前的迭代器副本
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_const_iterator operator--(int);

                    /**
                     * 迭代器加法，移动指定步数
                     * @param n 要移动的步数（可正可负）
                     * @return 移动后的新迭代器
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_const_iterator operator+(int n) const;
                    
                    /**
                     * 迭代器减法，移动指定步数（等价于加负数）
                     * @param n 要移动的步数
                     * @return 移动后的新迭代器
                     * @throw Eexception 当移动后下标越界时抛出异常
                     */
                    Aarray_const_iterator operator-(int n) const;

                    /**
                     * 迭代器差值计算，获取两个迭代器之间的距离
                     * @param other 另一个迭代器
                     * @return 两个迭代器的下标差值
                     * @throw Eexception 当两个迭代器不属于同一个数组时抛出异常
                     */
                    int operator-(const Aarray_const_iterator& other) const;

                    /**
                     * 相等比较运算符
                     * @param other 另一个迭代器
                     * @return 两个迭代器指向同一位置且属于同一数组时返回true
                     */
                    bool operator==(const Aarray_const_iterator& other) const;

                    /**
                     * 不等比较运算符
                     * @param other 另一个迭代器
                     * @return 两个迭代器不指向同一位置或不属于同一数组时返回true
                     */
                    bool operator!=(const Aarray_const_iterator& other) const;
            };

            /**
             * 获取普通迭代器的起始位置（指向第一个元素）
             * @return 指向第一个元素的迭代器
             * @throw Eexception 当数组为空时抛出异常
             */
            Aarray_iterator begin();
            
            /**
             * 获取普通迭代器的结束位置（指向最后一个元素的下一个位置）
             * @return 指向结束位置的迭代器
             */
            Aarray_iterator end();

            /**
             * 获取常量迭代器的起始位置（指向第一个元素），供常量对象使用
             * @return 指向第一个元素的常量迭代器
             * @throw Eexception 当数组为空时抛出异常
             */
            Aarray_const_iterator begin() const;

            /**
             * 获取常量迭代器的结束位置（指向最后一个元素的下一个位置），供常量对象使用
             * @return 指向结束位置的常量迭代器
             */
            Aarray_const_iterator end() const;

            /**
             * 用指定值填充整个数组
             * @param value 要填充的值
             */
            void fill(const E& value);
            
            /**
             * 填充指定范围内的值，[start_index,end_index)
             * @param value 需要填充的值
             * @param start_index 开始的下标（包含）
             * @param end_index 终止的下标（不包含）
             */
            void fill(const E& value,int start_index,int end_index);

            /**
             * 查找第一个匹配值的下标（全数组范围）
             * @param value 要查找的值
             * @return 找到时返回下标，未找到返回-1
             */
            int find_first(const E& value) const;

            /**
             * 查找指定范围内第一个匹配值的下标，[start_index,end_index)
             * @param value 要查找的值
             * @param start_index 开始的下标（包含）
             * @param end_index 终止的下标（不包含）
             * @return 找到时返回下标，未找到返回-1
             */
            int find_first(const E& value,int start_index,int end_index) const;

            /**
             * 查找指定范围内最后一个匹配值的下标，[start_index,end_index)
             * @param value 要查找的值
             * @param start_index 开始的下标（包含）
             * @param end_index 终止的下标（不包含）
             * @return 找到时返回下标，未找到或范围无效返回-1
             */
            int find_last(const E& value,int start_index,int end_index) const;

            /**
             * 查找整个数组中最后一个匹配值的下标
             * @param value 要查找的值
             * @return 找到时返回下标，未找到返回-1
             */            
            int find_last(const E& value);
        
            /**
             * 判断当前数组是否包含某个值
             * @param value 要检查的值
             * @return 包含该值返回true，否则返回false
             */
           bool contains(const E& value) const;

           /**
            * 交换两个下标对应的元素
            * @param index1 第一个元素的下标
            * @param index2 第二个元素的下标
            * @throw Eexception 当下标越界或两个下标相同时抛出异常
            */
           void swap(int index1,int index2);

           /**
            * 将整个数组逆序
            */
           void reverse();

           /**
            * 截取数组的一部分作为新数组返回，[start_index,end_index)
            * @param start_index 起始下标（包含）
            * @param end_index 终止下标（不包含）
            * @return 包含截取元素的新Aarray对象
            * @throw Eexception 当范围无效（越界或start >= end）时抛出异常
            */
           Aarray subArr(int start_index,int end_index);

           /**
            * 返回大小
            * @return 数组的大小
            */
           int size() const{
            return this->_length;
           }
    };
}

// 模板类特殊处理：头文件末尾包含源文件（确保编译时能找到实现）
#include "Aarray.cpp"

#endif // AARRAY_H