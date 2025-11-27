#ifndef DDEQUE_H
#define DDEQUE_H

#include "Vvector.h"
#include "Eexception.h"
#include "Ffunction.h"

namespace shed_std {
    /**
     * @brief 双端队列类，支持在两端高效插入和删除元素
     * @tparam T 队列中存储的元素类型
     */
    template <typename T>
    class Ddeque {
    private:
        static constexpr int _size_default = 1024;          ///< 默认初始大小
        static constexpr double _ratio_front = 0.25;       ///< 头部空闲空间占比

        Vvector<T> _data;                              ///< 存储数据的底层容器
        int _index_front = _size_default * _ratio_front; ///< 指向第一个有效元素的前一个位置的索引
        int _index_end = _index_front;

        /**
         * @brief 头部扩容机制，当头部无空闲空间时扩展头部容量
         * @note 扩容时会在现有数据前添加新的空闲空间，大小为当前数据长度的_ratio_front倍
         */
        void _expand();

    public:
        /**
         * @brief 默认构造函数，初始化一个具有默认大小的双端队列
         * @note 初始总容量为默认大小加上默认大小乘以头部空间占比
         */
        Ddeque();

        /**
         * @brief 构造函数，创建指定初始元素数量的双端队列
         * @param size 初始元素数量
         * @note 总容量为指定大小加上指定大小乘以头部空间占比，包含头部空闲空间和数据空间
         */
        Ddeque(int size);

        /**
         * @brief 拷贝构造函数，创建一个与另一个双端队列内容相同的新队列
         * @param other 被拷贝的双端队列对象
         */
        Ddeque(const Ddeque& other);

        /**
         * @brief 赋值运算符重载，将另一个双端队列的内容赋值给当前队列
         * @param other 被赋值的双端队列对象
         * @return 当前双端队列的引用
         * @note 自赋值时直接返回当前对象
         */
        Ddeque& operator=(const Ddeque& other);

        /**
         * @brief 相等运算符重载，判断两个双端队列是否相等
         * @param other 用于比较的另一个双端队列
         * @return 若两个队列大小相同且所有对应元素相等则返回true，否则返回false
         */
        bool operator==(const Ddeque& other) const;

        /**
         * @brief 不等运算符重载，判断两个双端队列是否不相等
         * @param other 用于比较的另一个双端队列
         * @return 若两个队列不相等则返回true，否则返回false
         */
        bool operator!=(const Ddeque& other) const;

        /**
         * @brief 获取双端队列中有效元素的数量
         * @return 有效元素的数量
         */
        int size() const;

        /**
         * @brief 判断双端队列是否为空
         * @return 若队列为空则返回true，否则返回false
         */
        bool empty() const;

        /**
         * @brief 压缩容量以适配实际元素数量，减少内存占用
         * @note 只保留必要的头部空间（实际元素数量乘以头部空间占比）和实际数据
         */
        void shrink_to_fit();

        /**
         * @brief 获取指定下标的元素引用（带范围检查）
         * @param index 要访问的元素下标（从0开始）
         * @return 指定下标的元素引用
         * @throw Eexception 当下标越界时抛出异常
         */
        T& at(int index);

        /**
         * @brief 获取指定下标的常量元素引用（带范围检查），供常量对象使用
         * @param index 要访问的元素下标（从0开始）
         * @return 指定下标的常量元素引用
         * @throw Eexception 当下标越界时抛出异常
         */
        const T& at(int index) const;

        /**
         * @brief 重载[]运算符，获取指定下标的元素引用
         * @param index 要访问的元素下标（从0开始）
         * @return 指定下标的元素引用
         * @note 内部调用at()方法，带范围检查
         */
        T& operator[](int index);

        /**
         * @brief 重载[]运算符，获取指定下标的常量元素引用，供常量对象使用
         * @param index 要访问的元素下标（从0开始）
         * @return 指定下标的常量元素引用
         * @note 内部调用const版本的at()方法，带范围检查
         */
        const T& operator[](int index) const;

        /**
         * @brief 获取队列头部的元素引用
         * @return 头部元素的引用
         * @throw Eexception 当队列为空时抛出异常
         */
        T& front();

        /**
         * @brief 获取队列头部的常量元素引用，供常量对象使用
         * @return 头部元素的常量引用
         * @throw Eexception 当队列为空时抛出异常
         */
        const T& front() const;

        /**
         * @brief 获取队列尾部的元素引用
         * @return 尾部元素的引用
         * @throw Eexception 当队列为空时抛出异常
         */
        T& back();

        /**
         * @brief 获取队列尾部的常量元素引用，供常量对象使用
         * @return 尾部元素的常量引用
         * @throw Eexception 当队列为空时抛出异常
         */
        const T& back() const;

        /**
         * @brief 清空队列中的所有有效元素，但保留头部空闲空间
         */
        void clear();

        /**
         * @brief 在指定下标位置插入元素
         * @param index 插入位置的下标（从0开始，插入后元素位于该下标处）
         * @param value 要插入的元素
         * @throw Eexception 当下标越界（小于0或大于当前元素数量）时抛出异常
         */
        void insert(int index, T& value);

        /**
         * @brief 擦除指定下标的元素
         * @param index 要擦除的元素下标（从0开始）
         * @throw Eexception 当下标越界时抛出异常
         */
        void erase(int index);

        /**
         * @brief 在队列尾部添加元素
         * @param value 要添加的元素
         */
        void push_back(T& value);

        /**
         * @brief 移除并销毁队列尾部的元素
         * @throw Eexception 当队列为空时抛出异常
         */
        void pop_back();

        /**
         * @brief 在队列头部添加元素
         * @param value 要添加的元素
         * @note 若头部无空闲空间，会先调用_expand()方法进行扩容
         */
        void push_front(T& value);

        /**
         * @brief 移除并销毁队列头部的元素
         * @throw Eexception 当队列为空时抛出异常
         */
        void pop_front();

        /**
         * @brief 交换两个指定下标的元素
         * @param index1 第一个元素的下标
         * @param index2 第二个元素的下标
         * @throw Eexception 当任何一个下标越界或两个下标相同时抛出异常
         */
        void swap(int index1, int index2);

        using Ddeque_iterator = typename Vvector<T>::Vvector_iterator;         ///< 迭代器类型定义
        using Ddeque_const_iterator = typename Vvector<T>::Vvector_const_iterator; ///< 常量迭代器类型定义

        /**
         * @brief 获取指向队列第一个元素的迭代器
         * @return 指向第一个元素的Ddeque_iterator
         */
        Ddeque_iterator begin();

        /**
         * @brief 获取指向队列第一个元素的常量迭代器，供常量对象使用
         * @return 指向第一个元素的Ddeque_const_iterator
         */
        Ddeque_const_iterator begin() const;

        /**
         * @brief 获取指向队列最后一个元素后一位的迭代器
         * @return 指向末尾的Ddeque_iterator
         */
        Ddeque_iterator end();

        /**
         * @brief 获取指向队列最后一个元素后一位的常量迭代器，供常量对象使用
         * @return 指向末尾的Ddeque_const_iterator
         */
        Ddeque_const_iterator end() const;

        /**
         * @brief 获取指向队列最后一个元素的反向迭代器
         * @return 指向最后一个元素的Ddeque_iterator
         */
        Ddeque_iterator rbegin();

        /**
         * @brief 获取指向队列最后一个元素的常量反向迭代器，供常量对象使用
         * @return 指向最后一个元素的Ddeque_const_iterator
         */
        Ddeque_const_iterator rbegin() const;

        /**
         * @brief 获取指向队列第一个元素前一位的反向迭代器
         * @return 指向反向末尾的Ddeque_iterator
         */
        Ddeque_iterator rend();

        /**
         * @brief 获取指向队列第一个元素前一位的常量反向迭代器，供常量对象使用
         * @return 指向反向末尾的Ddeque_const_iterator
         */
        Ddeque_const_iterator rend() const;

        /**
         * @brief 查找第一个等于指定值的元素下标
         * @param value 目标值
         * @return 第一个匹配元素的下标，未找到则返回-1
         */
        int find_first(const T& value) const;

        /**
         * @brief 在[start_index, end_index)范围内查找第一个等于指定值的元素下标
         * @param value 目标值
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 第一个匹配元素的下标，范围不合法或未找到则返回-1
         */
        int find_first(const T& value, int start_index, int end_index) const;

        /**
         * @brief 在[start_index, end_index)范围内查找第一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向第一个匹配元素的迭代器，范围不合法或未找到则返回end()
         */
        Ddeque_iterator find_first(const Ffunction<bool, const T&>& predicate, int start_index, int end_index);

        /**
         * @brief 在整个Ddeque中查找第一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @return 指向第一个匹配元素的迭代器，未找到则返回end()
         */
        Ddeque_iterator find_first(const Ffunction<bool, const T&>& predicate);

        /**
         * @brief 在[start_index, end_index)范围内查找第一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向第一个匹配元素的常量迭代器，范围不合法或未找到则返回end()
         */
        Ddeque_const_iterator find_first(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) const;

        /**
         * @brief 在整个Ddeque中查找第一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @return 指向第一个匹配元素的常量迭代器，未找到则返回end()
         */
        Ddeque_const_iterator find_first(const Ffunction<bool, const T&>& predicate) const;

        /**
         * @brief 判断Ddeque是否包含指定值
         * @param value 目标值
         * @return 包含则返回true，否则返回false
         */
        bool contains(const T& value) const;

        /**
         * @brief 判断[start_index, end_index)范围内是否包含指定值
         * @param value 目标值
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 包含则返回true，否则返回false
         */
        bool contains(const T& value, int start_index, int end_index) const;

        /**
         * @brief 判断Ddeque是否包含满足谓词条件的元素
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @return 包含则返回true，否则返回false
         */
        bool contains(const Ffunction<bool, const T&>& predicate) const;

        /**
         * @brief 判断[start_index, end_index)范围内是否包含满足谓词条件的元素
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 包含则返回true，否则返回false
         */
        bool contains(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) const;

        /**
         * @brief 查找最后一个等于指定值的元素下标
         * @param value 目标值
         * @return 最后一个匹配元素的下标，未找到则返回-1
         */
        int find_last(const T& value) const;

        /**
         * @brief 在[start_index, end_index)范围内查找最后一个等于指定值的元素下标
         * @param value 目标值
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 最后一个匹配元素的下标，范围不合法或未找到则返回-1
         */
        int find_last(const T& value, int start_index, int end_index) const;

        /**
         * @brief 在[start_index, end_index)范围内查找最后一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向最后一个匹配元素的迭代器，范围不合法则返回end()，未找到则返回指向end_index的迭代器
         */
        Ddeque_iterator find_last(const Ffunction<bool, const T&>& predicate, int start_index, int end_index);

        /**
         * @brief 在整个Ddeque中查找最后一个满足谓词条件的元素
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @return 指向最后一个匹配元素的迭代器，未找到则返回end()
         */
        Ddeque_iterator find_last(const Ffunction<bool, const T&>& predicate);

        /**
         * @brief 在[start_index, end_index)范围内查找最后一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @param start_index 起始索引（包含）
         * @param end_index 结束索引（不包含）
         * @return 指向最后一个匹配元素的常量迭代器，范围不合法则返回end()，未找到则返回指向end_index的迭代器
         */
        Ddeque_const_iterator find_last(const Ffunction<bool, const T&>& predicate, int start_index, int end_index) const;

        /**
         * @brief 在整个Ddeque中查找最后一个满足谓词条件的元素（常量版本）
         * @param predicate 谓词函数（参数为const T&，返回bool）
         * @return 指向最后一个匹配元素的常量迭代器，未找到则返回end()
         */
        Ddeque_const_iterator find_last(const Ffunction<bool, const T&>& predicate) const;

        /**
         * @brief 用指定值填充整个Ddeque的有效元素
         * @param value 填充值
         */
        void fill(const T& value);

        /**
         * @brief 在[start_index, end_index)范围内用指定值填充（Ddeque的逻辑范围）
         * @param value 填充值
         * @param start_index 起始索引（包含，Ddeque逻辑索引）
         * @param end_index 结束索引（不包含，Ddeque逻辑索引）
         * @note 若范围不合法，则不执行填充操作
         */
        void fill(const T& value, int start_index, int end_index);

        /**
         * @brief 将Ddeque的有效元素逆序排列（不影响头部空闲空间）
         */
        void reverse();

        /**
         * @brief 获取子Ddeque（[start_index, end_index)范围内的有效元素）
         * @param start_index 起始索引（包含，Ddeque逻辑索引）
         * @param end_index 结束索引（不包含，Ddeque逻辑索引）
         * @return 包含指定范围元素的新Ddeque对象
         * @throw Eexception 当范围不合法时抛出异常
         */
        Ddeque subDeque(int start_index, int end_index);
    };

}  // namespace shed_std

// 包含模板实现文件
#include "Ddeque.tpp"

#endif  // DDEQUE_H