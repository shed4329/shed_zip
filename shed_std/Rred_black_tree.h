#ifndef RRED_BLACK_TREE_H
#define RRED_BLACK_TREE_H

#include "Sstack.h"

namespace shed_std{
    // 红黑树节点
    template<typename T>
    struct Rred_black_tree_node
    {
        T data;
        Rred_black_tree_node* left_child;
        Rred_black_tree_node* right_child;
        Rred_black_tree_node* parent;
        bool is_red;//1=red,0=black
        
        /**
         * @brief 默认构造函数，用于初始化新节点。
         * @param val 节点存储的数据值。
         * 默认构造的新节点为红色。
         */
        Rred_black_tree_node(T val);
        
        /**
         * @brief 比较两个节点是否相等。
         */
        bool operator==(const Rred_black_tree_node& other) const;
        
        /**
         * @brief 比较两个节点是否不相等。
         */
        bool operator!=(const Rred_black_tree_node& other) const;
    };

    template<typename E>
    class Rred_black_tree{
        private:
            /**
             * 红黑树性质
             * 1.节点必须为红色或黑色
             * 2.NIL（空叶子节点）为黑色
             * 3.两个红色节点不能相邻，兄弟节点不算相邻
             * 4.根节点到NIL节点的每条路径上黑色节点数量相同
             * 5.头节点是黑色的
             */
            int _size = 0;

            /**
             * @brief 左旋辅助函数。
             * 以x节点为轴，将x的右孩子y往上提。
             * @param x 左旋操作的轴心节点。
             */
            void _left_rotate(Rred_black_tree_node<E>* x);

            /**
             * @brief 右旋辅助函数。
             * 以x节点为轴，将x的左孩子y往上提。
             * @param x 右旋操作的轴心节点。
             */
            void _right_rotate(Rred_black_tree_node<E>* x);

            /**
             * @brief 修复插入后的红黑树。
             * 当新插入的节点 z 和其父节点 p 都是红色时，调用此函数进行颜色翻转和旋转操作。
             * @param z 刚刚插入的红色节点。
             */
            void _fix_insert(Rred_black_tree_node<E>* z);

            /**
             * @brief 查找以x为根的子树中的最小节点（最左侧节点），用于删除操作。
             * @param x 子树的根节点。
             * @return 子树中最小节点的指针。
             */
            Rred_black_tree_node<E>* _tree_minimum(Rred_black_tree_node<E>* x);

            /**
             * @brief 查找以x为根的子树中的最小节点（最左侧节点），用于常量操作或迭代器。
             * @param x 子树的根节点。
             * @return 子树中最小节点的常量指针。
             */
            const Rred_black_tree_node<E>* _tree_minimum(Rred_black_tree_node<E>* x) const;

            /**
             * @brief 替换子树u为子树v（用于删除操作）。
             * 将节点 u 从其父节点 p 处移除，用节点 v 替代它的位置。v 继承 u 的父节点 p。
             * @param u 要被替换的节点。
             * @param v 替换 u 的节点。
             */
            void _red_black_transplant(Rred_black_tree_node<E>* u,Rred_black_tree_node<E>* v);

            /**
             * @brief 修复删除黑节点后可能产生的红黑树性质破坏（双黑问题）。
             * @param x 继承了双黑属性的节点。
             */
            void _red_black_delete_fixup(Rred_black_tree_node<E>* x);

            /**
             * @brief 红黑树删除操作的内部实现。
             * @param z 需要被删除的节点。
             */
            void _red_black_delete(Rred_black_tree_node<E>* z);
            
            /**
             * @brief 递归清理以 node 为根的子树中的所有节点。
             * @param node 当前节点。
             */
            void _clear_tree(Rred_black_tree_node<E>* node);

            /**
             * @brief 递归深度复制树结构。
             * @param other_node 源树中的当前节点。
             * @param parent_node 新树中的父节点。
             * @param other_nil 源树的 nil 哨兵。
             * @param new_nil 新树的 nil 哨兵。
             * @return 新树中复制的节点指针。
             */
            Rred_black_tree_node<E>* _copy_tree(const Rred_black_tree_node<E>* other_node, Rred_black_tree_node<E>* parent_node, const Rred_black_tree_node<E>* other_nil, Rred_black_tree_node<E>* new_nil);

            /**
             * @brief 递归比较两棵子树的结构、数据和颜色。
             * @param a 当前树的节点。
             * @param b 另一棵树的节点。
             * @param other_nil 另一棵树的 nil 哨兵。
             * @return 如果两棵子树在结构、数据和颜色上相等，返回 true。
             */
            bool _is_equal_recursive(const Rred_black_tree_node<E>* a, const Rred_black_tree_node<E>* b, const Rred_black_tree_node<E>* other_nil) const;

        public:
            // 根节点
            Rred_black_tree_node<E>* root;
            // 叶节点 (哨兵节点)
            Rred_black_tree_node<E>* nil;
            
            /**
             * @brief 默认构造函数。
             * 初始化哨兵节点 nil (黑色) 和根节点 root (指向 nil)。
             */
            Rred_black_tree();

            /**
             * @brief 析构函数。
             * 递归删除所有节点，并删除哨兵节点 nil。
             */
            ~Rred_black_tree();
            
            /**
             * @brief 拷贝构造函数 (深拷贝)。
             * @param other 另一个 Rred_black_tree 对象。
             */
            Rred_black_tree(const Rred_black_tree& other);

            /**
             * @brief 交换两个 Rred_black_tree 对象的所有内部状态 (root, nil, size)。
             * @param other 另一个要交换状态的 Rred_black_tree 对象。
             */
            void swap(Rred_black_tree& other);
            
            /**
             * @brief 拷贝赋值运算符 (使用 Copy-and-Swap idiom)。
             * @param other 另一个 Rred_black_tree 对象。
             * @return 当前对象的引用。
             */
            Rred_black_tree& operator=(const Rred_black_tree& other);

            /**
             * @brief 比较运算符：相等。
             * @param other 另一个 Rred_black_tree 对象。
             * @return 如果两棵树的大小、结构、数据和颜色都相同，返回 true。
             */
            bool operator==(const Rred_black_tree& other) const;
            
            /**
             * @brief 比较运算符：不等。
             * @param other 另一个 Rred_black_tree 对象。
             * @return 如果两棵树不相等，返回 true。
             */
            bool operator!=(const Rred_black_tree& other) const;

            /**
             * @brief 插入一个新元素到红黑树中。
             * @param data 要插入的元素值。
             */
            void insert(const E& data);

            /**
             * @brief 查找以x为根的子树中为key的节点。
             * @param key 要查找的元素值。
             * @param x 开始查找的子树根节点。**当第二个参数为 `nullptr` (默认值) 时，查找将从整个树的根节点 `this->root` 开始。**
             * @return 如果找到，返回节点指针；否则返回哨兵节点 `nil`。
             */
            Rred_black_tree_node<E>* find(const E& key, Rred_black_tree_node<E>* x = nullptr);

            /**
             * @brief 查找以x为根的子树中为key的节点 (const 版本)。
             * @param key 要查找的元素值。
             * @param x 开始查找的子树根节点。**当第二个参数为 `nullptr` (默认值) 时，查找将从整个树的根节点 `this->root` 开始。**
             * @return 如果找到，返回节点常量指针；否则返回哨兵节点 `nil`。
             */
            const Rred_black_tree_node<E>* find(const E& key, const Rred_black_tree_node<E>* x = nullptr) const;

            /**
             * @brief 检查红黑树中是否包含指定的值。
             * @param key 要检查的元素值。
             * @return 如果找到，返回 true；否则返回 false。
             */
            bool contains(const E& key) const;

            /**
             * @brief 删除红黑树中值为 value 的元素。
             * @param value 要删除的元素值。
             * @return 如果找到并成功删除，返回 true；如果未找到，返回 false。
             */
            bool remove(const E& value);

            /**
             * @brief 返回红黑树中的元素数量。
             */
            int size() const;

            /**
             * @brief 返回红黑树是否为空。
             */
            bool empty() const;


            // -----------------------------------------------------
            // 1. 定义 Rred_black_tree_const_iterator （常量迭代器）
            // -----------------------------------------------------
            class Rred_black_tree_const_iterator {
            protected:
                const Rred_black_tree_node<E>* _current;
                const Rred_black_tree_node<E>* _nil; 

                // 辅助函数：查找以x为根的子树中的最小节点（最左边）
                const Rred_black_tree_node<E>* _find_minimum(const Rred_black_tree_node<E>* x) const;

            public:
                /**
                 * @brief 构造函数。
                 * @param node 当前迭代器指向的节点。
                 * @param nil_node 树的哨兵节点。
                 */
                Rred_black_tree_const_iterator(const Rred_black_tree_node<E>* node, const Rred_black_tree_node<E>* nil_node);

                /**
                 * @brief 解引用 (const 版本)。
                 * @return 节点存储数据的常量引用。
                 */
                const E& operator*() const;

                /**
                 * @brief 成员访问 (const 版本)。
                 * @return 节点存储数据的常量指针。
                 */
                const E* operator->() const;

                /**
                 * @brief 比较两个迭代器是否相等。
                 */
                bool operator==(const Rred_black_tree_const_iterator& other) const;

                /**
                 * @brief 比较两个迭代器是否不相等。
                 */
                bool operator!=(const Rred_black_tree_const_iterator& other) const;

                /**
                 * @brief 前置递增 (in-order successor)。
                 * 移动到中序遍历的下一个节点。
                 */
                Rred_black_tree_const_iterator& operator++();

                /**
                 * @brief 后置递增。
                 * 移动到中序遍历的下一个节点。
                 */
                Rred_black_tree_const_iterator operator++(int);
                
                friend class Rred_black_tree_iterator;
            };
            
            // -----------------------------------------------------
            // 2. 定义 Rred_black_tree_iterator （可修改迭代器）
            // -----------------------------------------------------
            class Rred_black_tree_iterator : public Rred_black_tree_const_iterator {
            public:
                /**
                 * @brief 构造函数。
                 * @param node 当前迭代器指向的节点。
                 * @param nil_node 树的哨兵节点。
                 */
                Rred_black_tree_iterator(Rred_black_tree_node<E>* node, Rred_black_tree_node<E>* nil_node);

                /**
                 * @brief 解引用 (非 const 版本)。
                 * @return 节点存储数据的引用（可修改）。
                 */
                E& operator*() const;

                /**
                 * @brief 成员访问 (非 const 版本)。
                 * @return 节点存储数据的指针（可修改）。
                 */
                E* operator->() const;

                /**
                 * @brief 前置递增 (重用基类的逻辑)。
                 */
                Rred_black_tree_iterator& operator++();
                
                /**
                 * @brief 后置递增。
                 */
                Rred_black_tree_iterator operator++(int);
            };
            
            // -----------------------------------------------------
            // 3. 树的 begin() 和 end() 接口 (非 const)
            // -----------------------------------------------------

            /**
             * @brief 返回指向树中最小元素的迭代器（中序遍历的开始）。
             */
            Rred_black_tree_iterator begin();

            /**
             * @brief 返回指向树中最后一个元素之后位置的迭代器 (哨兵 nil)。
             */
            Rred_black_tree_iterator end();
            
            // -----------------------------------------------------
            // 4. 树的 begin() 和 end() 接口 (const)
            // -----------------------------------------------------

            /**
             * @brief 返回指向树中最小元素的常量迭代器。
             */
            Rred_black_tree_const_iterator begin() const;

            /**
             * @brief 返回指向树中最后一个元素之后位置的常量迭代器。
             */
            Rred_black_tree_const_iterator end() const;
        
    };


}

// 包含模板函数的定义
#include "Rred_black_tree.tpp"

#endif //RRED_BALCK_TREE_H