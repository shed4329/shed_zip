#ifndef LLIST_H  // 头文件保护符：防止重复包含
#define LLIST_H

namespace shed_std {

/**
 * 链表节点结构体
 * 存储数据和指向下一节点的指针
 */
template <typename T>
struct Node{
    Node* _next;  // 指向下一个节点的指针
    T _value;     // 当前节点存储的值

    /**
     * 节点默认构造函数
     * 初始化_next为nullptr，值为默认构造
     */
    Node();
    
    /**
     * 节点带参构造函数
     * @param next 下一个节点的指针
     * @param value 当前节点要存储的值
     */
    Node(Node* next, T value);
};


/**
 * 基础链表类（抽象基类）
 * 实现链表的核心基础功能，供派生类继承
 */
template <typename E>
class basic_list{
protected:
    Node<E>* _head = nullptr;  // 哨兵头节点（不存储实际数据，简化边界处理）
    Node<E>* _pre = nullptr;   // 当前操作的前驱节点（标记操作位置）
    int _length = 0;           // 链表中有效节点的数量

public:
    /**
     * 基础链表构造函数
     * 初始化哨兵节点，并将_pre指向哨兵节点
     */
    basic_list();

    /**
     * 基础链表析构函数
     * 释放所有有效节点和哨兵节点，避免内存泄漏
     */
    ~basic_list();
    
    /**
     * 清空链表中所有有效节点（保留哨兵节点）
     * 重置_length为0，_pre指向哨兵节点
     */
    void clear();

    /**
     * 在_pre指向的节点后插入新节点（当前操作位置前插入）
     * 插入后_pre位置不变，新节点成为_pre的直接后继
     * @param value 要插入的元素值
     * @return 始终返回true（插入逻辑无失败可能）
     */
    bool add(E value);

    /**
     * 删除_pre指向节点的下一个节点（当前操作位置的节点）
     * @return 成功删除返回true；_pre为空或无后继节点返回false
     */
    bool remove();

    /**
     * 将_pre指针向后移动一个位置（指向当前节点的后继）
     * @return 移动成功返回true；_pre为空或无后继节点返回false
     */
    bool move_to_next();

    /**
     * 将_pre指针重置到哨兵头节点
     * @return 成功重置返回true；_head为空（未初始化）返回false
     */
    bool reset_to_head();

    /**
     * 修改_pre指向节点的下一个节点的值
     * @param value 要更新的新值
     * @return 修改成功返回true；_pre为空或无后继节点返回false
     */
    bool set_cur_value(E value);

    /**
     * 获取_pre指向节点的下一个节点的值
     * @param value 用于存储结果的引用
     * @return 获取成功返回true；_pre为空或无后继节点返回false
     */
    bool get_cur_value(E& value) const;

    /**
     * 获取链表中有效节点的数量
     * @return 链表的长度（有效节点数）
     */
    const int length() const;

    /**
     * 判断链表是否为空
     * @return 链表为空（无有效节点）返回true，否则返回false
     */
    bool empty() const;
};


/**
 * 派生链表类（继承basic_list）
 * 扩展基础链表功能，支持反转、头插/头删、迭代器等
 */
template <typename E>
class Llist:public basic_list<E>{
public:
    /**
     * 派生类构造函数
     * 显式调用基类构造函数初始化链表
     */
    Llist();
    
    /**
     * 翻转链表（将所有有效节点的指向反转）
     * 例如：1→2→3 翻转后为 3→2→1
     * @return 翻转成功返回true；空链表或未初始化返回false
     */
    bool reverse();

    /**
     * 链表迭代器类
     * 支持遍历链表中的有效节点，提供指针式操作
     */
    class Llist_iterator{
    private:
        Node<E>* _cur;  // 当前指向的节点

    public:
        /**
         * 迭代器构造函数
         * @param node 迭代器初始指向的节点
         */
        Llist_iterator(Node<E>* node);

        /**
         * 解引用操作符
         * @return 返回当前节点存储的值的引用（可修改）
         */
        E& operator*() const;

        /**
         * 箭头操作符
         * @return 返回当前节点存储的值的指针（支持访问成员）
         */
        E* operator->() const;

        /**
         * 前置递增操作符（++it）
         * 移动迭代器到下一个节点，返回更新后的迭代器
         * @return 更新后的迭代器引用
         */
        Llist_iterator& operator++();

        /**
         * 后置递增操作符（it++）
         * 移动迭代器到下一个节点，返回原迭代器状态
         * @return 移动前的迭代器副本
         */
        Llist_iterator operator++(int);

        /**
         * 相等比较操作符
         * @param other 要比较的另一个迭代器
         * @return 两个迭代器指向同一节点返回true，否则返回false
         */
        bool operator==(const Llist_iterator& other) const;

        /**
         * 不等比较操作符
         * @param other 要比较的另一个迭代器
         * @return 两个迭代器指向不同节点返回true，否则返回false
         */
        bool operator!=(const Llist_iterator& other) const;
    };

    /**
     * 获取链表的起始迭代器（指向第一个有效节点）- 非const版本
     * @return 指向第一个有效节点的迭代器（可修改节点值）
     */
    Llist_iterator begin();

    /**
     * 获取链表的起始迭代器（指向第一个有效节点）- const版本
     * @return 指向第一个有效节点的迭代器（不可修改节点值）
     */
    Llist_iterator begin() const;

    /**
     * 获取链表的结束迭代器（指向nullptr）- 非const版本
     * @return 指向nullptr的迭代器（表示遍历结束）
     */
    Llist_iterator end();

    /**
     * 获取链表的结束迭代器（指向nullptr）- const版本
     * @return 指向nullptr的迭代器（表示遍历结束）
     */
    Llist_iterator end() const;

    /**
     * 在当前节点（_pre的后继节点）的后面插入新节点
     * 例如：_pre→A→B 插入C后变为 _pre→A→C→B
     * @param value 要插入的元素值
     * @return 插入成功返回true；_pre为空或无后继节点返回false
     */
    bool add_after(E value);

    /**
     * 拷贝构造函数（深拷贝）
     * 复制另一个链表的所有元素，生成独立的新链表
     * @param other 要拷贝的源链表（const引用，避免额外拷贝）
     */
    Llist(const Llist<E>& other);

    /**
     * 赋值运算符重载（深拷贝）
     * 将另一个链表的元素复制到当前链表，覆盖原有内容
     * @param other 要赋值的源链表（const引用）
     * @return 当前链表的引用（支持链式赋值，如a = b = c）
     */
    Llist<E>& operator=(const Llist<E>& other);

    /**
     * 在链表最前面插入节点（哨兵节点之后）
     * 插入后新节点成为第一个有效节点，时间复杂度O(1)
     * @param value 要插入的元素值
     */
    void add_at_front(E value);

    /**
     * 删除链表最前面的节点（第一个有效节点）
     * @return 删除成功返回true；链表为空（无有效节点）返回false
     */
    bool remove_at_front();
};

}  // 命名空间shed_std结束
#include "Llist.cpp"
#endif  // LLIST_H