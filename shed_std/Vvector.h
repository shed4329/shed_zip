#ifndef VVECTOR_H
#define VVECTOR_H

#include "Aarray.h"
#include "Eexception.h"
#include "Ffunction.h"

namespace shed_std {
    template <typename E>
    class Vvector {
    public:
        // 最大容量
        static const int MAX_CAPACITY = 0x40000000;
        // 最大大小，同最大容量
        static const int MAX_SIZE = 0x40000000;

        /**
         * 迭代器类，用于遍历Vvector中的元素
         */
        class Vvector_iterator {
        private:
            int _index = 0;          // 当前迭代器指向的索引
            Vvector<E>* _vec;        // 指向关联的Vvector对象
        public:
            /**
             * 迭代器构造函数
             * @param vec 关联的Vvector对象指针
             * @param index 初始索引位置
             * @throw Eexception 当index超出范围（<-1或>vec->_size）时抛出异常
             */
            Vvector_iterator(Vvector<E>* vec, int index);

            /**
             * 解引用操作符，获取当前索引指向的元素引用
             * @return 当前元素的引用
             * @throw Eexception 当索引无效时由at()方法抛出异常
             */
            E& operator*();

            /**
             * 成员访问操作符，获取当前元素的指针
             * @return 当前元素的指针
             * @throw Eexception 当索引无效时由at()方法抛出异常
             */
            E* operator->();

            /**
             * 前置自增运算符，移动到下一个元素
             * @return 自增后的迭代器引用
             * @throw Eexception 当下一个索引超出范围时抛出异常
             */
            Vvector_iterator& operator++();

            /**
             * 后置自增运算符，移动到下一个元素
             * @return 自增前的迭代器副本
             * @throw Eexception 当下一个索引超出范围时抛出异常
             */
            Vvector_iterator operator++(int);

            /**
             * 前置自减运算符，移动到上一个元素
             * @return 自减后的迭代器引用
             * @throw Eexception 当上一个索引超出范围（<-1）时抛出异常
             */
            Vvector_iterator& operator--();

            /**
             * 后置自减运算符，移动到上一个元素
             * @return 自减前的迭代器副本
             * @throw Eexception 当上一个索引超出范围（<-1）时抛出异常
             */
            Vvector_iterator operator--(int);

            /**
             * 相等比较运算符
             * @param other 另一个迭代器
             * @return 两个迭代器是否指向同一位置且关联同一Vvector对象
             */
            bool operator==(const Vvector_iterator& other) const;

            /**
             * 不等比较运算符
             * @param other 另一个迭代器
             * @return 两个迭代器是否不指向同一位置或不关联同一Vvector对象
             */
            bool operator!=(const Vvector_iterator& other) const;

            /**
             * 加法运算符，移动迭代器n个位置
             * @param n 移动的步数
             * @return 移动后的新迭代器
             * @throw Eexception 当移动后的索引超出范围时抛出异常
             */
            Vvector_iterator operator+(int n) const;

            /**
             * 减法运算符，移动迭代器-n个位置
             * @param n 移动的步数
             * @return 移动后的新迭代器
             * @throw Eexception 当移动后的索引超出范围时抛出异常
             */
            Vvector_iterator operator-(int n) const;

            /**
             * 迭代器差值运算符，计算两个迭代器的索引差
             * @param other 另一个迭代器
             * @return 两个迭代器的索引差值
             */
            int operator-(const Vvector_iterator& other) const;
        };

        /**
         * 常量迭代器类，用于遍历Vvector中的元素（只读）
         */
        class Vvector_const_iterator {
        private:
            int _index = 0;              // 当前迭代器指向的索引
            const Vvector<E>* _vec;      // 指向关联的常量Vvector对象
        public:
            /**
             * 常量迭代器构造函数
             * @param vec 关联的常量Vvector对象指针
             * @param index 初始索引位置
             * @throw Eexception 当index超出范围（<-1或>vec->_size）时抛出异常
             */
            Vvector_const_iterator(const Vvector<E>* vec, int index);

            /**
             * 解引用操作符，获取当前索引指向的元素常量引用
             * @return 当前元素的常量引用
             * @throw Eexception 当索引无效时由at()方法抛出异常
             */
            const E& operator*() const;

            /**
             * 成员访问操作符，获取当前元素的常量指针
             * @return 当前元素的常量指针
             * @throw Eexception 当索引无效时由at()方法抛出异常
             */
            const E* operator->() const;

            /**
             * 前置自增运算符，移动到下一个元素
             * @return 自增后的常量迭代器引用
             * @throw Eexception 当下一个索引超出范围时抛出异常
             */
            Vvector_const_iterator& operator++();

            /**
             * 后置自增运算符，移动到下一个元素
             * @return 自增前的常量迭代器副本
             * @throw Eexception 当下一个索引超出范围时抛出异常
             */
            Vvector_const_iterator operator++(int);

            /**
             * 前置自减运算符，移动到上一个元素
             * @return 自减后的常量迭代器引用
             * @throw Eexception 当上一个索引超出范围（<-1）时抛出异常
             */
            Vvector_const_iterator& operator--();

            /**
             * 后置自减运算符，移动到上一个元素
             * @return 自减前的常量迭代器副本
             * @throw Eexception 当上一个索引超出范围（<-1）时抛出异常
             */
            Vvector_const_iterator operator--(int);

            /**
             * 相等比较运算符
             * @param other 另一个常量迭代器
             * @return 两个迭代器是否指向同一位置且关联同一Vvector对象
             */
            bool operator==(const Vvector_const_iterator& other) const;

            /**
             * 不等比较运算符
             * @param other 另一个常量迭代器
             * @return 两个迭代器是否不指向同一位置或不关联同一Vvector对象
             */
            bool operator!=(const Vvector_const_iterator& other) const;

            /**
             * 加法运算符，移动迭代器n个位置
             * @param n 移动的步数
             * @return 移动后的新常量迭代器
             * @throw Eexception 当移动后的索引超出范围时抛出异常
             */
            Vvector_const_iterator operator+(int n) const;

            /**
             * 减法运算符，移动迭代器-n个位置
             * @param n 移动的步数
             * @return 移动后的新常量迭代器
             * @throw Eexception 当移动后的索引超出范围时抛出异常
             */
            Vvector_const_iterator operator-(int n) const;

            /**
             * 迭代器差值运算符，计算两个迭代器的索引差
             * @param other 另一个常量迭代器
             * @return 两个迭代器的索引差值
             */
            int operator-(const Vvector_const_iterator& other) const;
        };

        /**
         * 构造函数，默认容量为1024，大小为0
         */
        Vvector();

        /**
         * 构造函数，指定初始大小，容量为适合该大小的2的幂次
         * @param size 初始大小
         * @throw Eexception 当size为负数或超过MAX_CAPACITY时抛出异常
         */
        Vvector(int size);

        /**
         * 拷贝构造函数，实现深拷贝
         * @param other 另一个Vvector对象
         */
        Vvector(const Vvector& other);

        /**
         * 赋值运算符，实现深拷贝
         * @param other 另一个Vvector对象
         * @return 当前对象的引用
         */
        Vvector& operator=(const Vvector& other);

        /**
         * 相等比较运算符
         * @param other 另一个Vvector对象
         * @return 两个对象是否相等（大小相同且元素均相同）
         */
        bool operator==(const Vvector& other) const;

        /**
         * 不等比较运算符
         * @param other 另一个Vvector对象
         * @return 两个对象是否不相等
         */
        bool operator!=(const Vvector& other) const;

        /**
         * 获取当前Vvector的大小
         * @return 元素个数
         */
        int length() const;

        /**
         * 获取当前Vvector的大小（与length()功能相同）
         * @return 元素个数
         */
        int size() const;

        /**
         * 判断Vvector是否为空
         * @return 若大小为0则返回true，否则返回false
         */
        bool empty() const;

        /**
         * 获取指定索引的元素引用（带越界检查）
         * @param index 元素索引
         * @return 元素的引用
         * @throw Eexception 当index<0或index>=_size时抛出异常
         */
        E& at(int index);

        /**
         * 获取指定索引的元素常量引用（带越界检查）
         * @param index 元素索引
         * @return 元素的常量引用
         * @throw Eexception 当index<0或index>=_size时抛出异常
         */
        const E& at(int index) const;

        /**
         * 下标运算符，通过索引访问元素（调用at()实现）
         * @param index 元素索引
         * @return 元素的引用
         * @throw Eexception 当index<0或index>=_size时抛出异常
         */
        E& operator[](int index);

        /**
         * 常量下标运算符，通过索引访问元素（调用at()实现）
         * @param index 元素索引
         * @return 元素的常量引用
         * @throw Eexception 当index<0或index>=_size时抛出异常
         */
        const E& operator[](int index) const;

        /**
         * 在末尾添加元素
         * @param value 要添加的元素值
         * @throw Eexception 当Vvector已达到MAX_SIZE时抛出异常；扩容失败时也可能抛出异常
         */
        void push_back(const E& value);

        /**
         * 移除末尾元素（不释放容量）
         * @throw Eexception 当Vvector为空时抛出异常
         */
        void pop_back();

        /**
         * 在指定索引位置插入元素
         * @param index 插入位置索引
         * @param value 要插入的元素值
         * @throw Eexception 当index<0或index>_size时抛出异常；Vvector已达MAX_SIZE时抛出异常；扩容失败时也可能抛出异常
         */
        void insert(int index, const E& value);

        /**
         * 删除指定索引位置的元素
         * @param index 要删除的元素索引
         * @throw Eexception 当index<0或index>=_size时抛出异常
         */
        void erase(int index);

        /**
         * 获取第一个元素的引用
         * @return 第一个元素的引用
         * @throw Eexception 当Vvector为空时抛出异常
         */
        E& front();

        /**
         * 获取第一个元素的常量引用
         * @return 第一个元素的常量引用
         * @throw Eexception 当Vvector为空时抛出异常
         */
        const E& front() const;

        /**
         * 获取最后一个元素的引用
         * @return 最后一个元素的引用
         * @throw Eexception 当Vvector为空时抛出异常
         */
        E& back();

        /**
         * 获取最后一个元素的常量引用
         * @return 最后一个元素的常量引用
         * @throw Eexception 当Vvector为空时抛出异常
         */
        const E& back() const;

        /**
         * 获取指向起始元素的迭代器
         * @return 起始迭代器
         */
        Vvector_iterator begin();

        /**
         * 获取指向起始元素的常量迭代器
         * @return 起始常量迭代器
         */
        Vvector_const_iterator begin() const;

        /**
         * 获取指向末尾（最后一个元素后一位）的迭代器
         * @return 末尾迭代器
         */
        Vvector_iterator end();

        /**
         * 获取指向末尾（最后一个元素后一位）的常量迭代器
         * @return 末尾常量迭代器
         */
        Vvector_const_iterator end() const;

        /**
         * 获取指向最后一个元素的反向起始迭代器
         * @return 反向起始迭代器
         */
        Vvector_iterator rbegin();

        /**
         * 获取指向最后一个元素的反向起始常量迭代器
         * @return 反向起始常量迭代器
         */
        Vvector_const_iterator rbegin() const;

        /**
         * 获取指向第一个元素前一位的反向末尾迭代器
         * @return 反向末尾迭代器
         */
        Vvector_iterator rend();

        /**
         * 获取指向第一个元素前一位的反向末尾常量迭代器
         * @return 反向末尾常量迭代器
         */
        Vvector_const_iterator rend() const;

        /**
         * 清空Vvector（仅重置大小为0，不释放容量，不调用元素析构函数）
         */
        void clear();

        /**
         * 获取Vvector的最大容量限制
         * @return 最大元素个数
         */
        int max_size() const;

        /**
         * 获取当前容量
         * @return 容量大小
         */
        int capacity() const;

        /**
         * 缩减容量至适合当前大小的2的幂次（当容量大于所需时）
         */
        void shrink_to_fit();

        /**
         * 预留至少能容纳指定大小的容量（容量保持为2的幂次）
         * @param size 所需容纳的元素个数
         * @throw Eexception 当size为负数或超过MAX_CAPACITY时抛出异常
         */
        void reserve(int size);

        /**
         * 查找第一个等于指定值的元素下标
         * @param value 目标值
         * @return 第一个匹配元素的下标，未找到则返回-1
         */
        int find_first(const E& value) const;

        /**
         * 在[start_index, end_index)范围内查找第一个等于指定值的元素下标
         * @param value 目标值
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 第一个匹配元素的下标，范围不合法或未找到则返回-1
         */
        int find_first(const E& value, int start_index, int end_index) const;

        /**
         * 在[start_index, end_index)范围内查找第一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向第一个匹配元素的迭代器，范围不合法或未找到则返回end()
         */
        Vvector_iterator find_first(const Ffunction<bool, const E&>& predicate, int start_index, int end_index);

        /**
         * 在整个Vvector中查找第一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @return 指向第一个匹配元素的迭代器，未找到则返回end()
         */
        Vvector_iterator find_first(const Ffunction<bool, const E&>& predicate);

        /**
         * 在[start_index, end_index)范围内查找第一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向第一个匹配元素的常量迭代器，范围不合法或未找到则返回end()
         */
        Vvector_const_iterator find_first(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) const;

        /**
         * 在整个Vvector中查找第一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @return 指向第一个匹配元素的常量迭代器，未找到则返回end()
         */
        Vvector_const_iterator find_first(const Ffunction<bool, const E&>& predicate) const;

        /**
         * 判断Vvector是否包含指定值
         * @param value 目标值
         * @return 包含则返回true，否则返回false
         */
        bool contains(const E& value) const;

        /**
         * 判断[start_index, end_index)范围内是否包含指定值
         * @param value 目标值
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 包含则返回true，否则返回false
         */
        bool contains(const E& value, int start_index, int end_index) const;

        /**
         * 判断Vvector是否包含满足谓词条件的元素
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @return 包含则返回true，否则返回false
         */
        bool contains(const Ffunction<bool, const E&>& predicate) const;

        /**
         * 判断[start_index, end_index)范围内是否包含满足谓词条件的元素
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 包含则返回true，否则返回false
         */
        bool contains(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) const;

        /**
         * 查找最后一个等于指定值的元素下标
         * @param value 目标值
         * @return 最后一个匹配元素的下标，未找到则返回-1
         */
        int find_last(const E& value) const;

        /**
         * 在[start_index, end_index)范围内查找最后一个等于指定值的元素下标
         * @param value 目标值
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 最后一个匹配元素的下标，范围不合法或未找到则返回-1
         */
        int find_last(const E& value, int start_index, int end_index) const;

        /**
         * 在[start_index, end_index)范围内查找最后一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向最后一个匹配元素的迭代器，范围不合法则返回end()，未找到则返回指向end_index的迭代器
         */
        Vvector_iterator find_last(const Ffunction<bool, const E&>& predicate, int start_index, int end_index);

        /**
         * 在整个Vvector中查找最后一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @return 指向最后一个匹配元素的迭代器，未找到则返回end()
         */
        Vvector_iterator find_last(const Ffunction<bool, const E&>& predicate);

        /**
         * 在[start_index, end_index)范围内查找最后一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向最后一个匹配元素的常量迭代器，范围不合法则返回end()，未找到则返回指向end_index的迭代器
         */
        Vvector_const_iterator find_last(const Ffunction<bool, const E&>& predicate, int start_index, int end_index) const;

        /**
         * 在整个Vvector中查找最后一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const E&，返回bool）
         * @return 指向最后一个匹配元素的常量迭代器，未找到则返回end()
         */
        Vvector_const_iterator find_last(const Ffunction<bool, const E&>& predicate) const;

        /**
         * 用指定值填充整个Vvector
         * @param value 填充值
         */
        void fill(const E& value);

        /**
         * 在[start_index, end_index)范围内用指定值填充
         * @param value 填充值
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @note 若范围不合法，则不执行填充操作
         */
        void fill(const E& value, int start_index, int end_index);

        /**
         * 交换两个指定索引位置的元素
         * @param index1 第一个元素的索引
         * @param index2 第二个元素的索引
         * @note 若索引不合法或相同，则不执行交换操作
         */
        void swap(int index1, int index2);

        /**
         * 将Vvector中的元素逆序排列
         */
        void reverse();

        /**
         * 获取子Vvector（[start_index, end_index)范围内的元素）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 包含指定范围元素的新Vvector对象
         * @throw Eexception 当范围不合法时抛出异常
         */
        Vvector subVec(int start_index, int end_index);

        
        /**
         * 更改容器的size大小
         * @param count 容器的新大小
         */
        void resize(int count);

    private:
        int _capacity;       // 容量（2的幂次）
        int _size = 0;       // 实际元素个数
        Aarray<E> _array;    // 内部存储数组

        /**
         * 计算适合指定大小的2的幂次容量
         * @param size 所需容纳的元素个数
         * @return 适合的容量（2的幂次）
         * @throw Eexception 当size<0或size>MAX_CAPACITY时抛出异常
         */
        int _get_fitting_capacity(int size);

        /**
         * 扩容操作（容量翻倍）
         * @throw Eexception 当容量已超过MAX_CAPACITY/2时抛出异常（无法扩容）
         */
        void _expand();

        /**
         * 检查[start, end)区间是否合法
         * @param start 起始索引
         * @param end 结束索引
         * @return 若区间合法（0<=start<end<=_size）则返回true，否则返回false
         */
        bool _is_valid_range(int start, int end) const;
    };

#include "Vvector.tpp"
}

#endif // VVECTOR_H