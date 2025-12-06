#ifndef HHEAP_H
#define HHEAP_H

#include "Ccomparator.h"
#include "Vvector.h"
#include "Eexception.h"

namespace shed_std {

/**
 * @brief 二叉堆容器类，默认实现为大根堆
 * @tparam T 堆中元素的类型
 * @tparam Comparator 比较器类型，用于定义元素的优先级（默认使用less<T>，即大根堆）
 */
template <typename T, typename Comparator = less<T>>
class Hheap {
private:
    Vvector<T> _data;       ///< 存储堆元素的底层容器
    Comparator _comparator; ///< 用于比较元素优先级的比较器

    /**
     * @brief 向上调整堆（插入元素时使用）
     * @details 将指定索引的元素向上移动，直到满足堆的性质
     * @param index 需要调整的元素索引
     */
    void shift_up(int index);

    /**
     * @brief 向下调整堆（删除元素时使用）
     * @details 将指定索引的元素向下移动，直到满足堆的性质
     * @param index 需要调整的元素索引
     */
    void shift_down(int index);

public:
    /**
     * @brief 默认构造函数
     * @details 初始化空堆，使用默认的比较器
     */
    Hheap();

    /**
     * @brief 带自定义比较器的构造函数
     * @param comparator 自定义的比较器对象
     */
    Hheap(const Comparator& comparator);

    /**
     * @brief 从向量构造堆
     * @details 将输入向量中的元素堆化，构建一个满足堆性质的容器
     * @param vec 用于初始化堆的向量
     * @param comparator 比较器对象（默认使用默认构造的比较器）
     */
    Hheap(const Vvector<T>& vec, const Comparator& comparator = Comparator());

    /**
     * @brief 析构函数
     * @details 默认析构，由编译器自动生成
     */
    ~Hheap() = default;

    /**
     * @brief 相等比较运算符
     * @param other 另一个Hheap对象
     * @return 若两个堆的元素和比较器均相同则返回true，否则返回false
     */
    bool operator==(const Hheap& other) const;

    /**
     * @brief 不等比较运算符
     * @param other 另一个Hheap对象
     * @return 若两个堆不相等则返回true，否则返回false
     */
    bool operator!=(const Hheap& other) const;

    /**
     * @brief 获取堆中元素的数量
     * @return 堆的大小（元素个数）
     */
    int size() const;

    /**
     * @brief 检查堆是否为空
     * @return 若堆为空则返回true，否则返回false
     */
    bool empty() const;

    /**
     * @brief 获取堆顶元素（优先级最高的元素）
     * @return 堆顶元素的常量引用
     * @throw Eexception 当堆为空时抛出异常
     */
    const T& top() const;

    /**
     * @brief 向堆中插入元素
     * @details 将元素添加到堆的末尾，然后通过向上调整维护堆的性质
     * @param value 要插入的元素值
     */
    void push(const T& value);

    /**
     * @brief 移除堆顶元素
     * @details 将堆顶元素与最后一个元素交换，移除最后一个元素，然后通过向下调整维护堆的性质
     * @throw Eexception 当堆为空时抛出异常
     */
    void pop();

    /**
     * @brief 清空堆中所有元素
     * @details 重复调用pop()直到堆为空
     */
    void clear();

    /**
     * @brief 获取当前使用的比较器
     * @return 比较器的常量引用
     */
    const Comparator& get_comparator() const;
};

} // namespace shed_std

#include "Hheap.tpp"

#endif // HHEAP_H