#ifndef QQUEUE_H
#define QQUEUE_H

#include "Vvector.h"
#include "Eexception.h"

namespace shed_std {
    /**
     * @brief 队列类（FIFO 先进先出）
     * @tparam T 队列存储的元素类型（需支持拷贝构造）
     */
    template <typename T>
    class Qqueue {
    private:
        // 存储队列元素的动态数组
        Vvector<T> _data;
        // 队头偏移量：标记当前有效队头的位置（跳过已逻辑删除的元素）
        int _offset = 0;
        // 整理比例：当偏移量占总容量的比例超过此值时，触发冗余元素清理
        static const double _trim_ratio;

        /**
         * @brief 延迟整理：移除偏移量前的冗余元素，优化空间利用率
         * @details 当 _offset 超过 _data.size() * _trim_ratio 时触发，
         *          复制有效元素到新数组，重置 _offset 为 0
         */
        void _trim();

    public:
        // 默认构造函数
        Qqueue() = default;

        /**
         * @brief 拷贝构造函数
         * @param other 待拷贝的队列对象
         * @details 仅复制 other 中的有效元素（跳过偏移量前的冗余元素），
         *          新队列的 _offset 重置为 0
         */
        Qqueue(const Qqueue& other);

        /**
         * @brief 赋值运算符重载
         * @param other 待赋值的队列对象
         * @return Qqueue<T>& 赋值后的当前对象引用
         * @details 先清空当前队列，再复制 other 中的有效元素，
         *          确保赋值后与 other 的有效元素完全一致
         */
        Qqueue& operator=(const Qqueue& other);
        
        /**
         * @brief 队尾插入元素
         * @param value 待插入的元素（const 引用，避免拷贝开销）
         * @details 直接在 _data 尾部插入，时间复杂度 O(1)
         */
        void push(const T& value);

        /**
         * @brief 队头删除元素
         * @throw Eexception 当队列为空时调用，抛出空队列删除异常
         * @details 通过增加 _offset 实现逻辑删除（避免元素移动），
         *          若偏移量达到整理比例，触发 _trim() 清理冗余元素，
         *          平均时间复杂度 O(1)
         */
        void pop();

        /**
         * @brief 访问队头元素（非 const 版本，支持修改）
         * @return T& 队头元素的引用
         * @throw Eexception 当队列为空时调用，抛出空队列访问异常
         * @details 直接通过 _offset 定位有效队头，时间复杂度 O(1)
         */
        T& front();

        /**
         * @brief 访问队头元素（const 版本，不支持修改）
         * @return const T& 队头元素的 const 引用
         * @throw Eexception 当队列为空时调用，抛出空队列访问异常
         * @details 用于 const 对象或只读场景，时间复杂度 O(1)
         */
        const T& front() const;

        /**
         * @brief 访问队尾元素（非 const 版本，支持修改）
         * @return T& 队尾元素的引用
         * @throw Eexception 当队列为空时调用，抛出空队列访问异常
         * @details 直接访问 _data 的尾部（始终为有效队尾），时间复杂度 O(1)
         */
        T& back();

        /**
         * @brief 访问队尾元素（const 版本，不支持修改）
         * @return const T& 队尾元素的 const 引用
         * @throw Eexception 当队列为空时调用，抛出空队列访问异常
         * @details 用于 const 对象或只读场景，时间复杂度 O(1)
         */
        const T& back() const;
        
        /**
         * @brief 判断队列是否为空
         * @return bool 空返回 true，非空返回 false
         * @details 通过有效元素数量（_data.size() - _offset）是否为 0 判断
         */
        bool empty() const;

        /**
         * @brief 获取队列中有效元素的数量
         * @return int 有效元素个数（>= 0）
         * @details 计算方式：总元素数 - 队头偏移量（排除冗余元素）
         */
        int size() const;

        /**
         * @brief 清空队列所有元素
         * @details 清空 _data 数组并重置 _offset 为 0，
         *          释放所有内存，时间复杂度 O(n)（n 为总元素数）
         */
        void clear();

        /**
         * @brief 判断两个队列是否相等
         * @param other 待比较的另一个队列
         * @return bool 相等返回 true，否则返回 false
         * @details 先比较有效元素数量，数量一致再逐元素比较，
         *          仅判断有效元素（忽略冗余元素）
         */
        bool operator==(const Qqueue& other) const;

        /**
         * @brief 判断两个队列是否不相等
         * @param other 待比较的另一个队列
         * @return bool 不相等返回 true，否则返回 false
         * @details 是 operator== 的取反，逻辑上与 !operator==(other) 一致
         */
        bool operator!=(const Qqueue& other) const;
    };

   

} 

#include "Qqueue.cpp"//导入实现

#endif // QQUEUE_H