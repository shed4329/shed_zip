#ifndef HHASHMAP_H
#define HHASHMAP_H

#include "type_traits.h"
#include "pair.h"
#include "Llist.h"
#include "Rred_black_tree.h"
#include "Sstring.h"
#include "Aarray.h"
#include "Eexception.h"

namespace shed_std {

// -------------------------------------------------------------------
// 1. 默认哈希函数对象：DefaultHash
// -------------------------------------------------------------------

/**
 * @brief 默认哈希函数对象，采用MurmurHash 3算法变体
 * @tparam K 哈希键的类型
 */
template <typename K>
struct DefaultHash {
    static constexpr unsigned long long C1 = 0xff51afd7ed558ccdULL;  // 哈希常数1
    static constexpr unsigned long long C2 = 0xc4ceb9fe1a85ec53ULL;  // 哈希常数2
    static constexpr unsigned long long R1 = 33;                     // 右移位数

    /**
     * @brief 计算键的哈希值
     * @param key 待哈希的键
     * @return 哈希值（32位整数）
     */
    int operator()(const K& key) const {
        unsigned long long h = static_cast<unsigned long long>(key);

        h ^= h >> R1;
        h *= C1;
        h ^= h >> R1;
        h *= C2;
        h ^= h >> R1;

        return static_cast<int>(h & 0xFFFFFFFF);  // 截取低32位
    }
};

/**
 * @brief 字符串类型的哈希函数特化，采用Java String哈希实现
 */
template <>
struct DefaultHash<Sstring> {
    static const int PRIME = 31;  // 哈希质数

    /**
     * @brief 计算字符串的哈希值
     * @param key 待哈希的字符串
     * @return 哈希值
     */
    int operator()(const Sstring& key) const {
        if (key.size() == 0) {
            return 0;
        }
        int hash = 0;
        for (int i = 0; i < key.size(); ++i) {
            // 转换为无符号字符避免符号扩展问题
            hash = PRIME * hash + static_cast<int>(static_cast<unsigned char>(key[i]));
        }
        return hash;
    }
};

// -------------------------------------------------------------------
// 2. Hhashmap 主模板类（使用链表解决哈希冲突）
// -------------------------------------------------------------------

/**
 * @brief 哈希表容器，使用链表解决哈希冲突
 * @tparam K 键类型
 * @tparam V 值类型
 * @tparam Hash 哈希函数类型（默认使用DefaultHash）
 * @tparam Enable 用于SFINAE的占位符
 */
template <typename K, typename V, typename Hash = DefaultHash<K>, typename Enable = void>
class Hhashmap {
private:
    Aarray<Llist<pair<K, V>>> _data;  // 桶数组（每个桶是链表）
    Hash _hash;                       // 哈希函数对象
    int _size = 0;                    // 元素数量
    static constexpr int DEFAULT_INITIAL_CAPACITY = 16;  // 默认初始容量（2的幂）
    static constexpr double LOAD_FACTOR = 0.75;          // 负载因子阈值

    /**
     * @brief 检查负载因子并在需要时自动扩容
     */
    void _auto_expand();

    /**
     * @brief 重新哈希到新容量
     * @param new_capacity 新的桶数量（必须为2的幂）
     */
    void rehash(int new_capacity);

public:
    /**
     * @brief 迭代器类，用于遍历哈希表元素
     */
    class Hhashmap_iterator {
    private:
        using List = shed_std::Llist<shed_std::pair<K, V>>;
        using ListIterator = typename List::Llist_iterator;

        Hhashmap<K, V, Hash>* _map;       // 指向所属哈希表
        int _bucket_index;                // 当前桶索引
        ListIterator _list_iterator;      // 当前链表迭代器

        /**
         * @brief 查找下一个非空桶
         */
        void find_next_valid_bucket();

    public:
        /**
         * @brief 构造函数
         * @param map 所属哈希表指针
         * @param index 桶索引
         * @param it 链表迭代器
         */
        Hhashmap_iterator(Hhashmap<K, V, Hash>* map, int index, ListIterator it);

        /**
         * @brief 默认构造函数（用于end()标记）
         */
        Hhashmap_iterator();

        /**
         * @brief 解引用操作符
         * @return 指向当前元素的引用
         */
        pair<K, V>& operator*() const;

        /**
         * @brief 成员访问操作符
         * @return 指向当前元素的指针
         */
        pair<K, V>* operator->() const;

        /**
         * @brief 前置递增
         * @return 递增后的迭代器
         */
        Hhashmap_iterator& operator++();

        /**
         * @brief 后置递增
         * @return 递增前的迭代器副本
         */
        Hhashmap_iterator operator++(int);

        /**
         * @brief 相等比较
         * @param other 另一个迭代器
         * @return 若相等则返回true
         */
        bool operator==(const Hhashmap_iterator& other) const;

        /**
         * @brief 不等比较
         * @param other 另一个迭代器
         * @return 若不等则返回true
         */
        bool operator!=(const Hhashmap_iterator& other) const;
    };

    /**
     * @brief 无参构造函数，初始化默认容量
     */
    Hhashmap();

    /**
     * @brief 析构函数
     */
    ~Hhashmap() = default;

    /**
     * @brief 拷贝构造函数
     * @param other 被拷贝的哈希表
     */
    Hhashmap(const Hhashmap& other);

    /**
     * @brief 拷贝赋值运算符
     * @param other 被赋值的哈希表
     * @return 自身引用
     */
    Hhashmap& operator=(const Hhashmap& other);

    /**
     * @brief 相等比较运算符
     * @param other 另一个哈希表
     * @return 若相等则返回true
     */
    bool operator==(const Hhashmap& other) const;

    /**
     * @brief 不等比较运算符
     * @param other 另一个哈希表
     * @return 若不等则返回true
     */
    bool operator!=(const Hhashmap& other) const;

    /**
     * @brief 计算键对应的桶索引
     * @param key 目标键
     * @return 桶索引
     */
    int get_index(const K& key) const;

    /**
     * @brief 获取指定索引的桶（链表）
     * @param index 桶索引
     * @return 链表指针
     */
    Llist<pair<K, V>>* get_list(int index);

    /**
     * @brief 获取指定索引的桶（链表）（const版本）
     * @param index 桶索引
     * @return 常量链表指针
     */
    const Llist<pair<K, V>>* get_list(int index) const;

    /**
     * @brief 插入或更新键值对
     * @param key 键
     * @param value 值
     */
    void insert(const K& key, const V& value);

    /**
     * @brief 查找键对应的值
     * @param key 目标键
     * @return 指向值的指针（若未找到则返回nullptr）
     */
    V* get(const K& key);


    /**
     * @brief 下标访问运算符（未找到时插入空值）
     * @param key 目标键
     * @return 值的引用
     * @throw Eexception 当键不存在时
     */
    V& operator[](const K& key);

    /**
     * @brief 获取元素数量
     * @return 元素数量
     */
    int size() const;

    /**
     * @brief 检查哈希表是否为空
     * @return 若为空则返回true
     */
    int empty() const;

    /**
     * @brief 获取起始迭代器
     * @return 指向第一个元素的迭代器
     */
    Hhashmap_iterator begin();

    /**
     * @brief 获取结束迭代器
     * @return 指向末尾的迭代器
     */
    Hhashmap_iterator end();

    /**
     * @brief 清空哈希表所有元素
     */
    void clear();

    /**
     * @brief 删除指定键的元素
     * @param key 目标键
     */
    void erase(const K& key);

    /**
     * @brief 访问指定键的元素,未找到时抛出异常
     * @param key 目标键
     * @return 值的引用
     */
    V& at(const K& key);

    /**
     * @brief 获取当前桶数量
     * @return 桶数量
     */
    int bucket_size();


    /**
     * @brief 查找key对应的迭代器，找不到返回end
     * @param 键的值
     * @return 迭代器
     */
    Hhashmap_iterator find(const K& key);

};

// -------------------------------------------------------------------
// 3. Hhashmap 偏特化（使用红黑树优化可比较类型）
// -------------------------------------------------------------------

/**
 * @brief 哈希表偏特化版本，对可完全排序的值类型使用红黑树解决冲突
 * @tparam K 键类型
 * @tparam V 值类型（必须是可完全排序的）
 * @tparam Hash 哈希函数类型
 */
template <typename K, typename V, typename Hash>
class Hhashmap<K, V, Hash, enable_if_type<is_totally_ordered<K>::value>> {
private:
    Aarray<Rred_black_tree<pair<K, V>>> _data;  // 桶数组（每个桶是红黑树）
    Hash _hash;                                 // 哈希函数对象
    int _size = 0;                              // 元素数量
    static constexpr int DEFAULT_INITIAL_CAPACITY = 16;  // 默认初始容量（2的幂）
    static constexpr double LOAD_FACTOR = 0.75;          // 负载因子阈值

    /**
     * @brief 检查负载因子并在需要时自动扩容
     */
    void _auto_expand();

    /**
     * @brief 重新哈希到新容量
     * @param new_capacity 新的桶数量（必须为2的幂）
     */
    void rehash(int new_capacity);

public:
    /**
     * @brief 普通迭代器类
     */
    class Hhashmap_iterator {
    private:
        using Tree = shed_std::Rred_black_tree<shed_std::pair<K, V>>;
        using TreeIterator = typename Tree::Rred_black_tree_iterator;

        Hhashmap<K, V, Hash>* _map;       // 指向所属哈希表
        int _bucket_index;                // 当前桶索引
        TreeIterator _tree_iterator;      // 当前红黑树迭代器

        /**
         * @brief 查找下一个非空桶
         */
        void find_next_valid_bucket();

    public:
        /**
         * @brief 构造函数
         * @param map 所属哈希表指针
         * @param index 桶索引
         * @param it 红黑树迭代器
         */
        Hhashmap_iterator(Hhashmap<K, V, Hash>* map, int index, TreeIterator it);

        /**
         * @brief 默认构造函数（用于end()标记）
         */
        Hhashmap_iterator();

        /**
         * @brief 解引用操作符
         * @return 指向当前元素的引用
         */
        pair<K, V>& operator*() const;

        /**
         * @brief 成员访问操作符
         * @return 指向当前元素的指针
         */
        pair<K, V>* operator->() const;

        /**
         * @brief 前置递增
         * @return 递增后的迭代器
         */
        Hhashmap_iterator& operator++();

        /**
         * @brief 后置递增
         * @return 递增前的迭代器副本
         */
        Hhashmap_iterator operator++(int);

        /**
         * @brief 相等比较
         * @param other 另一个迭代器
         * @return 若相等则返回true
         */
        bool operator==(const Hhashmap_iterator& other) const;

        /**
         * @brief 不等比较
         * @param other 另一个迭代器
         * @return 若不等则返回true
         */
        bool operator!=(const Hhashmap_iterator& other) const;
    };

    /**
     * @brief 常量迭代器类
     */
    class Hhashmap_const_iterator {
    private:
        using Tree = shed_std::Rred_black_tree<shed_std::pair<K, V>>;
        using TreeConstIterator = typename Tree::Rred_black_tree_const_iterator;

        const Hhashmap<K, V, Hash>* _map;  // 指向所属哈希表（常量）
        int _bucket_index;                 // 当前桶索引
        TreeConstIterator _tree_iterator;  // 当前红黑树常量迭代器

        /**
         * @brief 查找下一个非空桶
         */
        void find_next_valid_bucket();

    public:
        /**
         * @brief 构造函数
         * @param map 所属哈希表指针（常量）
         * @param index 桶索引
         * @param it 红黑树常量迭代器
         */
        Hhashmap_const_iterator(const Hhashmap<K, V, Hash>* map, int index, TreeConstIterator it);

        /**
         * @brief 默认构造函数（用于end()标记）
         */
        Hhashmap_const_iterator();

        /**
         * @brief 从普通迭代器构造常量迭代器
         * @param other 普通迭代器
         */
        Hhashmap_const_iterator(const Hhashmap_iterator& other);

        /**
         * @brief 解引用操作符
         * @return 指向当前常量元素的引用
         */
        const pair<K, V>& operator*() const;

        /**
         * @brief 成员访问操作符
         * @return 指向当前常量元素的指针
         */
        const pair<K, V>* operator->() const;

        /**
         * @brief 前置递增
         * @return 递增后的迭代器
         */
        Hhashmap_const_iterator& operator++();

        /**
         * @brief 后置递增
         * @return 递增前的迭代器副本
         */
        Hhashmap_const_iterator operator++(int);

        /**
         * @brief 相等比较
         * @param other 另一个迭代器
         * @return 若相等则返回true
         */
        bool operator==(const Hhashmap_const_iterator& other) const;

        /**
         * @brief 不等比较
         * @param other 另一个迭代器
         * @return 若不等则返回true
         */
        bool operator!=(const Hhashmap_const_iterator& other) const;
    };

    /**
     * @brief 无参构造函数，初始化默认容量
     */
    Hhashmap();

    /**
     * @brief 析构函数
     */
    ~Hhashmap() = default;

    /**
     * @brief 拷贝构造函数
     * @param other 被拷贝的哈希表
     */
    Hhashmap(const Hhashmap& other);

    /**
     * @brief 拷贝赋值运算符
     * @param other 被赋值的哈希表
     * @return 自身引用
     */
    Hhashmap& operator=(const Hhashmap& other);

    /**
     * @brief 相等比较运算符
     * @param other 另一个哈希表
     * @return 若相等则返回true
     */
    bool operator==(const Hhashmap& other) const;

    /**
     * @brief 不等比较运算符
     * @param other 另一个哈希表
     * @return 若不等则返回true
     */
    bool operator!=(const Hhashmap& other) const;

    /**
     * @brief 计算键对应的桶索引
     * @param key 目标键
     * @return 桶索引
     */
    int get_index(const K& key) const;

    /**
     * @brief 获取指定索引的桶（红黑树）
     * @param index 桶索引
     * @return 红黑树指针
     */
    Rred_black_tree<pair<K, V>>* get_tree(int index);

    /**
     * @brief 获取指定索引的桶（红黑树）（const版本）
     * @param index 桶索引
     * @return 常量红黑树指针
     */
    const Rred_black_tree<pair<K, V>>* get_tree(int index) const;

    /**
     * @brief 插入或更新键值对
     * @param key 键
     * @param value 值
     */
    void insert(const K& key, const V& value);

    /**
     * @brief 查找键对应的值
     * @param key 目标键
     * @return 指向值的指针（若未找到则返回nullptr）
     */
    V* get(const K& key);

    /**
     * @brief 查找键对应的值（const版本）
     * @param key 目标键
     * @return 指向常量值的指针（若未找到则返回nullptr）
     */
    const V* get(const K& key) const;

    /**
     * @brief 下标访问运算符（未找到插入空值）
     * @param key 目标键
     * @return 值的引用
     * @throw Eexception 当键不存在时
     */
    V& operator[](const K& key);

    /**
     * @brief 获取元素数量
     * @return 元素数量
     */
    int size() const;

    /**
     * @brief 检查哈希表是否为空
     * @return 若为空则返回true
     */
    int empty() const;

    /**
     * @brief 清空哈希表所有元素
     */
    void clear();

    /**
     * @brief 删除指定键的元素
     * @param key 目标键
     */
    void erase(const K& key);

    /**
     * @brief 访问指定键的元素，未找到时抛出异常
     * @param key 目标键
     * @return 值的引用
     */
    V& at(const K& key);

    /**
     * @brief 访问指定键的元素，未找到时抛出异常
     * @param key 目标键
     * @return 值的常量引用
     */
    const V& at(const K& key) const;

    /**
     * @brief 获取当前桶数量
     * @return 桶数量
     */
    int bucket_size();

    /**
     * @brief 获取起始迭代器
     * @return 指向第一个元素的迭代器
     */
    Hhashmap_iterator begin();

    /**
     * @brief 获取结束迭代器
     * @return 指向末尾的迭代器
     */
    Hhashmap_iterator end();

    /**
     * @brief 获取起始常量迭代器
     * @return 指向第一个元素的常量迭代器
     */
    Hhashmap_const_iterator begin() const;

    /**
     * @brief 获取结束常量迭代器
     * @return 指向末尾的常量迭代器
     */
    Hhashmap_const_iterator end() const;

    /**
     * @brief 获取起始常量迭代器（cbegin）
     * @return 指向第一个元素的常量迭代器
     */
    Hhashmap_const_iterator cbegin() const;

    /**
     * @brief 获取结束常量迭代器（cend）
     * @return 指向末尾的常量迭代器
     */
    Hhashmap_const_iterator cend() const;

     /**
     * @brief 查找key对应的迭代器，找不到返回end
     * @param 键的值
     * @return 迭代器
     */
    Hhashmap_iterator find(const K& key);

    /**
     * @brief 查找key对应的迭代器，找不到返回end
     * @param 键的值
     * @return 常量迭代器，未找到返回end
     */
    Hhashmap_const_iterator find(const K& key) const;
};

}  // namespace shed_std

#include "Hhashmap.tpp"

#endif  // HHASHMAP_H