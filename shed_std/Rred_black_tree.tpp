#ifndef RRED_BLACK_TREE_TPP
#define RRED_BLACK_TREE_TPP

#include "Rred_black_tree.h"

namespace shed_std{
    template<typename T>
    Rred_black_tree_node<T>::Rred_black_tree_node(T val):data(val),is_red(true),left_child(nullptr),right_child(nullptr),parent(nullptr){}

    template<typename T>
    bool Rred_black_tree_node<T>::operator==(const Rred_black_tree_node& other) const{
        // 是否相等
        return data == other.data && left_child == other.left_child && right_child == other.right_child && is_red == other.is_red;
    }

    template<typename T>
    bool Rred_black_tree_node<T>::operator!=(const Rred_black_tree_node& other) const{
        // 是否不相等
        return !(*this == other);
    }


    // ==========================================================
    // Rred_black_tree 私有成员函数实现
    // ==========================================================

    template<typename E>
    void Rred_black_tree<E>::_left_rotate(Rred_black_tree_node<E>* x){
        Rred_black_tree_node<E>* y = x->right_child;

        // 1. 调节y的左子树b
        x->right_child = y->left_child;// x->a,b
        if(y->left_child != nil){
            y->left_child->parent = x;//b <- x
        }

        // 2.帮y认爸爸
        y->parent = x->parent;//y认爷爷当爸爸
        if(x->parent == nil){
            this->root = y;// 没有先人，那么y当祖宗
        }else if(x == x->parent->left_child){//爷爷认个新儿子
            // 父亲原来是爷爷的左孩子
            x->parent->left_child = y;
        }else{
            x->parent->right_child = y;
        }
        // 3.爸儿对调
        y->left_child = x;
        x->parent = y;
    }

    template<typename E>
    void Rred_black_tree<E>::_right_rotate(Rred_black_tree_node<E>* x){
        Rred_black_tree_node<E>* y = x->left_child;

        // 1. 调节y的右子树b
        x->left_child = y->right_child;// x->a,b
        if(y->right_child != nil){
            y->right_child->parent = x;//b <- x
        }

        // 2.帮y认爸爸
        y->parent = x->parent;//y认爷爷当爸爸
        if(x->parent == nil){
            this->root = y;// 没有先人，那么y当祖宗
        }else if(x == x->parent->left_child){//爷爷认个新儿子
            // 父亲原来是爷爷的左孩子
            x->parent->left_child = y;
        }else{
            x->parent->right_child = y;
        }
        // 3.爸儿对调
        y->right_child = x;
        x->parent = y;
    }

    template<typename E>
    void Rred_black_tree<E>::_fix_insert(Rred_black_tree_node<E>* z){
        while(z->parent->is_red){
            // 只有当z的父节点为红色，才有必要修复，因为只可能违背相邻红色
            Rred_black_tree_node<E>* p = z->parent; //父节点p
            Rred_black_tree_node<E>* g = p->parent; //祖父节点g(g不为nil,因为p是红色，所以p一定有父亲)

            if(p == g->left_child){
                // p是g的左孩子
                Rred_black_tree_node<E>* u = g->right_child;// 叔叔节点

                // case 1:叔叔u是红色的
                //          g(?)                        g(b)
                //      p(r)    u(r)        -->     p(b)    u(b)
                //  z(r)                        z(b)
                // r = red, b = black
                if(u->is_red){
                    // 1. 父亲颜色翻转
                    p->is_red = false; // p(b)
                    // 2. 叔叔变黑
                    u->is_red = false;
                    // 3. 爷爷变红
                    g->is_red = true;
                    // 4.向上检查
                    z = g;
                }else{
                    // case 2&3:叔叔是黑色的，需要旋转
                    // case 2: z在内侧,LR型转为LL型，也就是case3
                    //      g(b)                  g(b)              g(b)               g(b)
                    //      /   \               /      \   rotate  /   \              /   \ 
                    //    p(r)  u(b)    -->  p(r)(temp)u(b) -->  z(r)  u(b) -->    p(r)   u(b)
                    //      \                   \               /                  /
                    //      z(r)                z(r)           P(r)             z(r)
                    if(z == p->right_child){
                        Rred_black_tree_node<E>* temp = p;  //z移动到p
                        _left_rotate(temp);
                        // 接下来交换z和p 
                        // temp = p;
                        p = z;
                        z = temp;
                    }
                    // case 3: z是外侧节点(L-L型)
                    //      g(b)                g(b)           g(r)             p(b)
                    //      /   \               /  \          /   \    rotate   /   \ 
                    //    p(r)  u(b)  ->     p(b)  u(b) ->  p(b)  u(b)  --->  z(r)   g(r)
                    //    /                   /             /                           \ 
                    //  z(r)                z(r)          z(r)                          u(b)
                    p->is_red = false; // z变黑
                    g->is_red = true; // g 变红
                    _right_rotate(g);//对g右旋
                }
            }else { // II. p 是 g 的右孩子 (R-*) (镜像对称)
                Rred_black_tree_node<E>* u = g->left_child; // 叔叔节点 u

                // ==========================================================
                // Case 1: 叔叔 u 是红色的 (颜色翻转，向上迭代)
                // ==========================================================
                if (u->is_red) {
                    p->is_red = false;
                    u->is_red = false;
                    g->is_red = true;
                    z = g;
                } else {
                    // ==========================================================
                    // Case 2 & 3: 叔叔 u 是黑色的 (需要旋转)
                    // ==========================================================
                    
                    // Case 2: z在内侧 (R-L 型) -> 转换为 R-R 型
                    if (z == p->left_child) {
                        Rred_black_tree_node<E>* temp = p;
                        _right_rotate(temp);
                        
                        // 指针互换，为 Case 3 做准备
                        // temp = p
                        p = z;          // p 指向 Z_new
                        z = temp;       // z 指向 P
                    }
                    
                    // Case 3: z是外侧节点 (R-R型)
                    p->is_red = false; // p 变黑
                    g->is_red = true;  // g 变红
                    _left_rotate(g);    // 对 g 左旋
                    // 修复完成，跳出循环
                }
            }
        }
        this->root->is_red = false;//头必须要是黑色的
    }

    template<typename E>
    Rred_black_tree_node<E>* Rred_black_tree<E>::_tree_minimum(Rred_black_tree_node<E>* x){
        // 查找以x为根的子树中的最小节点（删除辅助函数）
        while(x->left_child != nil){
            x = x->left_child;
        }
        return x;
    }

    template<typename E>
    const Rred_black_tree_node<E>* Rred_black_tree<E>::_tree_minimum(Rred_black_tree_node<E>* x) const{
        while(x->left_child != nil){
            x = x->left_child;
        }
        return x;
    }

    template<typename E>
    void Rred_black_tree<E>::_red_black_transplant(Rred_black_tree_node<E>* u,Rred_black_tree_node<E>* v){
        // 替换子树u为v（用于删除)
        if(u->parent == nil){
            this->root = v;
        }else if(u->parent->left_child == u){
            u->parent->left_child = v;
        }else{
            u->parent->right_child = v;
        }
        v->parent = u->parent;
    }

    template<typename E>
    void Rred_black_tree<E>::_red_black_delete_fixup(Rred_black_tree_node<E>* x){
        while(x != this->root && !x->is_red){
            // x不为根节点且x为黑色时
            Rred_black_tree_node<E>* w;//兄弟节点

            if(x == x->parent->left_child){
                w = x->parent->right_child;
                if(w->is_red){//case 1： 兄弟是红色的,转为为case2，3，4
                    //   p           p         p(r)  rotate  w(b)
                    //  / \     ->  / \   ->   / \     --->  /   
                    // x(b)w(r)   x(b) w(b) x(b)  w(b)     p(r) 
                    //                                      / \ 
                    //                                    x(b) w_new// the updated w
                    w->is_red = false;
                    x->parent->is_red = true;
                    _left_rotate(x->parent);
                    w = x->parent->right_child;//更新w为x的兄弟
                }
                if(!w->left_child->is_red && !w->right_child->is_red){//case 2: w和w的子节点都是黑色的
                    //    p                 p               x_new // updated x
                    //   / \               / \ 
                    // x(b) w(b)    ->  x(b)  w(r)    ->
                    //      / \               / \ 
                    //    m(b) n(b)        m(b)  n(b)
                    w->is_red = true;
                    x = x->parent;
                }else{
                    if(!w->right_child->is_red){//case 3:w 是黑色，左红右黑,转换为case4
                        //      p                   p               p                  p
                        //     / \                 / \             / \      rotate    / \ 
                        //   x(b) w(b)      ->   x(b) w(b)  ->  x(b)  w(r)   --->  x(b)  m(b)
                        //        /  \                / \             / \                  \ 
                        //      m(r)  n(b)          m(b) n(b)      m(b)  n(b)               w(r)
                        //                                                                   \  
                        //                                                                   n(b)
                        w->left_child->is_red = false;
                        w->is_red = true;
                        _right_rotate(w);
                        w = x->parent->right_child; // update w
                    }
                    // case4:w 是黑色，右孩子是红色
                    //      p(?)            p(?)               p(b)             p(b)                    w(?)
                    //     / \              / \                /  \            /  \                     /  \   
                    //  x(b)  w(b)    -> x(b)  w(?)    ->   x(b)  w(?)    -> x(b)  w(?)         ->    p(b)  n(b)
                    //        / \              /  \                / \             / \                / \ 
                    //       m   n(r)         m    n(r)           m   n(r)        m   n(b)          x(b) m
                    w -> is_red = x->parent->is_red;
                    w->parent->is_red = false;
                    w->right_child->is_red = false;
                    _left_rotate(x->parent);
                    x = this->root; // 终止循环修复
                }
            }else{
                // 镜像
                w = x->parent->left_child; // 兄弟节点 w 是左孩子
                
                if (w->is_red) { // Case 1： 兄弟是红色的，转为为 Case 2, 3, 4
                    // 镜像 Case 1：将 w 变黑，p 变红，对 p 进行右旋
                    w->is_red = false;
                    x->parent->is_red = true;
                    _right_rotate(x->parent); // 镜像旋转：右旋
                    w = x->parent->left_child; // 更新 w 为 x 的新兄弟
                }

                // 接下来处理 Case 2, 3, 4（此时 w 必定是黑色）

                // 假设采用标准 Case 2 条件：w 的两个孩子都是黑色
                if (!w->left_child->is_red && !w->right_child->is_red) { 
                    // Case 2: w 和 w 的子节点都是黑色的
                    // 功能：双黑属性上浮到父节点 p
                    w->is_red = true;
                    x = x->parent; // 双黑上浮
                } else {
                    if (!w->left_child->is_red) { 
                        // Case 3：w 是黑色，外侧孩子 w_L 是黑色，内侧孩子 w_R 是红色 (内红外黑)
                        // 镜像 Case 3：内侧孩子 w_R 变黑，w 变红，对 w 左旋，转化为 Case 4
                        w->right_child->is_red = false; // w 的内侧孩子 w_R (红) 变黑
                        w->is_red = true;               // w 变红
                        _left_rotate(w);                // 镜像旋转：对 w 左旋
                        w = x->parent->left_child;      // 更新 w，完成 Case 3 转化
                    }

                    // Case 4：w 是黑色，外侧孩子 w_L 是红色 (或由 Case 3 转化而来)
                    // 功能：彻底消除双黑属性
                    w->is_red = x->parent->is_red;      // w 继承 p 的颜色
                    w->parent->is_red = false;          // p 变黑，吸收 BB
                    w->left_child->is_red = false;      // w 的外侧孩子 w_L (红) 变黑
                    _right_rotate(x->parent);           // 镜像旋转：对 p 右旋
                    x = this->root;                     // 修复完成，终止循环
                }
            }
        }
    }

    template<typename E>
    void Rred_black_tree<E>::_red_black_delete(Rred_black_tree_node<E>* z){
        Rred_black_tree_node<E>* y = z;
        Rred_black_tree_node<E>* x;
        bool y_original_color = y->is_red;

        if(z->left_child == nil){ // z没有左孩子
            x = z->right_child;
            _red_black_transplant(z,z->right_child); //换成右孩子
        }else if(z->right_child == nil){
            x = z->left_child;
            _red_black_transplant(z,z->left_child);//换成左孩子
        }else{
            y = _tree_minimum(z->right_child); // 找到右子树的min，辅助BST大小维持
            y_original_color = y->is_red;
            x = y->right_child;
            
            // x 的父节点必须指向 y 的父节点 (在 y 离开原位前)
            // 无论是 nil 还是普通节点，我们都需要记录 y 的父节点
            Rred_black_tree_node<E>* x_parent = y->parent; 
            
            if (y->parent != z) { // y 不直接是 z 的孩子
                // 1. 用 x 替换 y 的位置
                _red_black_transplant(y, y->right_child); 
                // 2. y 继承 z 的右孩子
                y->right_child = z->right_child;
                y->right_child->parent = y;
            }
            // else: y 是 z 的直接孩子，x 会自动继承 y 的父节点（即 z 的位置），不需要额外的 transplant

            // 3. 用 y 替换 z 的位置
            _red_black_transplant(z, y); 
            
            // 4. y 继承 z 的左孩子和颜色
            y->left_child = z->left_child;
            y->left_child->parent = y;
            y->is_red = z->is_red;

            // 5. 确保 x 的父指针正确（在 transplant 之后）
            if (x != nil) {
                // 修复x的父亲
                if (x_parent == z) { // y 是左孩子
                    x->parent = y;
                } else {
                    x->parent = x_parent;
                }
            }
        }

        delete z;
        //如果原来是黑色，遇到了双黑问题，修复
        if(!y_original_color){
            _red_black_delete_fixup(x);
        }
    }

    template<typename E>
    void Rred_black_tree<E>::_clear_tree(Rred_black_tree_node<E>* node) {
        if (node != nil) {
            _clear_tree(node->left_child);
            _clear_tree(node->right_child);
            delete node;
        }
    }

    template<typename E>
    Rred_black_tree_node<E>* Rred_black_tree<E>::_copy_tree(const Rred_black_tree_node<E>* other_node, Rred_black_tree_node<E>* parent_node, const Rred_black_tree_node<E>* other_nil, Rred_black_tree_node<E>* new_nil) {
        if (other_node == other_nil) {
            return new_nil;
        }
        
        Rred_black_tree_node<E>* new_node = new Rred_black_tree_node<E>(other_node->data);
        new_node->is_red = other_node->is_red;
        new_node->parent = parent_node;

        new_node->left_child = _copy_tree(other_node->left_child, new_node, other_nil, new_nil);
        new_node->right_child = _copy_tree(other_node->right_child, new_node, other_nil, new_nil);
        
        return new_node;
    }

    template<typename E>
    bool Rred_black_tree<E>::_is_equal_recursive(const Rred_black_tree_node<E>* a, const Rred_black_tree_node<E>* b, const Rred_black_tree_node<E>* other_nil) const {
        bool a_is_nil = (a == this->nil);
        bool b_is_nil = (b == other_nil);

        // 1. 如果两者都是 nil (哨兵)，则匹配
        if (a_is_nil && b_is_nil) {
            return true;
        }
        // 2. 如果其中一个是 nil，另一个不是，则结构不匹配
        if (a_is_nil || b_is_nil) {
            return false;
        }

        // 3. 比较当前节点的数据和颜色
        // 注意：这里只比较数据和颜色，不比较指针
        if (a->data != b->data || a->is_red != b->is_red) {
            return false;
        }
        
        // 4. 递归比较左右子树
        return _is_equal_recursive(a->left_child, b->left_child, other_nil) && 
            _is_equal_recursive(a->right_child, b->right_child, other_nil);
    }

    // ==========================================================
    // Rred_black_tree 公有成员函数实现
    // ==========================================================

    template<typename E>
    Rred_black_tree<E>::Rred_black_tree(){
        nil = new Rred_black_tree_node<E>(E());
        nil->is_red = false;
        root = nil;
    }

    template<typename E>
    Rred_black_tree<E>::~Rred_black_tree() {
        // 1. 递归删除所有数据节点
        _clear_tree(this->root);
        // 2. 删除哨兵节点 nil
        delete nil;
        this->root = nullptr;
        this->nil = nullptr;
    }

    template<typename E>
    Rred_black_tree<E>::Rred_black_tree(const Rred_black_tree& other) {
        // 1. 初始化新的哨兵 nil
        this->nil = new Rred_black_tree_node<E>(E());
        this->nil->is_red = false;
        
        // 2. 初始化大小
        this->_size = other._size;

        // 3. 深度复制树结构
        this->root = _copy_tree(other.root, this->nil, other.nil, this->nil);
    }

    template<typename E>
    void Rred_black_tree<E>::swap(Rred_black_tree& other) {
        // 手动交换指针和大小
        Rred_black_tree_node<E>* temp_root = this->root;
        this->root = other.root;
        other.root = temp_root;

        Rred_black_tree_node<E>* temp_nil = this->nil;
        this->nil = other.nil;
        other.nil = temp_nil;

        int temp_size = this->_size;
        this->_size = other._size;
        other._size = temp_size;
    }

    template<typename E>
    Rred_black_tree<E>& Rred_black_tree<E>::operator=(const Rred_black_tree& other) {
        // 1. 创建一个临时副本 (调用拷贝构造函数)
        Rred_black_tree temp(other);
        
        // 2. 交换 *this 和 temp 的内部状态 (调用自定义 swap)
        this->swap(temp);

        // 3. temp 离开作用域时，会自动调用析构函数，安全地清理 *this 的旧数据
        return *this;
    }

    template<typename E>
    bool Rred_black_tree<E>::operator==(const Rred_black_tree& other) const {
        // 1. 比较大小
        if (_size != other._size) {
            return false;
        }
        // 2. 递归比较结构、数据和颜色
        // 注意：我们必须传入 other.nil 作为对比的哨兵
        return _is_equal_recursive(this->root, other.root, other.nil); 
    }

    template<typename E>
    bool Rred_black_tree<E>::operator!=(const Rred_black_tree& other) const {
        return !(*this == other);
    }

    template<typename E>
    void Rred_black_tree<E>::insert(const E& data){
        // 1.创建新节点z
        Rred_black_tree_node<E>* new_node = new Rred_black_tree_node<E>(data);
        new_node->left_child = nil;
        new_node->right_child = nil;//还没有孩子

        Rred_black_tree_node<E>* y = nil; // y最终是z的父节点
        Rred_black_tree_node<E>* x = this->root; // 从根节点开始查找

        // 2.找到z的插入位置(BST插入)
        while(x!=nil){
            y = x;
            // 需要支持operator<
            if(new_node->data < x->data){
                // 走左边
                x = x->left_child;
            } else{
                // 走右边
                x = x->right_child;
            }
        }

        // 3. 把z挂到y下
        new_node->parent = y;
        if(y == nil){
            this->root = new_node; // 空树，新节点当根节点
        }else if(new_node->data < y->data){
            //放左边
            y ->left_child = new_node;
        }else{
            y ->right_child = new_node;
        }
        
        _fix_insert(new_node);
        _size++;
    }

    template<typename E>
    Rred_black_tree_node<E>* Rred_black_tree<E>::find(const E& key, Rred_black_tree_node<E>* x){
        // 修正：检查 x 是否为 nullptr，如果是则从 root 开始
        Rred_black_tree_node<E>* current = (x == nullptr) ? this->root : x;
        
        while(current!=nil){
            if(key<current->data){
                // 左边
                current = current->left_child;
            }else if(key > current->data){
                current = current->right_child;
            }else{
                return current;
            }
        }
        return nil;//没找到
    }

    template<typename E>
    const Rred_black_tree_node<E>* Rred_black_tree<E>::find(const E& key, const Rred_black_tree_node<E>* x) const{
        // 修正：参数 x 必须是 const 指针，默认值为 nullptr
        // 修正：检查 x 是否为 nullptr，如果是则从 root 开始
        const Rred_black_tree_node<E>* current = (x == nullptr) ? this->root : x;
        
        while(current!=nil){
            if(key<current->data){
                // 左边
                current = current->left_child;
            }else if(key > current->data){
                current = current->right_child;
            }else{
                return current;
            }
        }
        return nil;//没找到
    }

    template<typename E>
    bool Rred_black_tree<E>::contains(const E& key) const{
        return find(key)!=nil;
    }

    template<typename E>
    bool Rred_black_tree<E>::remove(const E& value){
        // 1.查找需要删除的节点
        Rred_black_tree_node<E>* z = find(value);
        // 2. 如果未找到，直接返回
        if (z == this->nil || z == nullptr) {
            return false;
        }

        // 3. 调用内部删除函数
        // 之前讨论的复杂逻辑都在这个内部函数中完成
        _red_black_delete(z); 

        // 4. 更新树的状态
        _size--;
        return true;
    }

    template<typename E>
    int Rred_black_tree<E>::size() const{
        return _size;
    }

    template<typename E>
    bool Rred_black_tree<E>::empty() const{
        return _size == 0;
    }


    // ==========================================================
    // Rred_black_tree_const_iterator 实现
    // ==========================================================

    template<typename E>
    const Rred_black_tree_node<E>* Rred_black_tree<E>::Rred_black_tree_const_iterator::_find_minimum(const Rred_black_tree_node<E>* x) const {
        const Rred_black_tree_node<E>* temp = x;
        while(temp->left_child != _nil){
            temp = temp->left_child;
        }
        return temp;
    }

    template<typename E>
    Rred_black_tree<E>::Rred_black_tree_const_iterator::Rred_black_tree_const_iterator(const Rred_black_tree_node<E>* node, const Rred_black_tree_node<E>* nil_node) 
        : _current(node), _nil(nil_node) {}

    template<typename E>
    const E& Rred_black_tree<E>::Rred_black_tree_const_iterator::operator*() const {
        // 解引用 (const 版本)
        return _current->data;
    }

    template<typename E>
    const E* Rred_black_tree<E>::Rred_black_tree_const_iterator::operator->() const {
        // 成员访问 (const 版本)
        return &(_current->data);
    }

    template<typename E>
    bool Rred_black_tree<E>::Rred_black_tree_const_iterator::operator==(const Rred_black_tree_const_iterator& other) const {
        // 比较
        return _current == other._current;
    }

    template<typename E>
    bool Rred_black_tree<E>::Rred_black_tree_const_iterator::operator!=(const Rred_black_tree_const_iterator& other) const {
        // 比较
        return _current != other._current;
    }

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_const_iterator& Rred_black_tree<E>::Rred_black_tree_const_iterator::operator++() {
        // 前置递增 (in-order successor)
        if (_current == _nil) {
            return *this;
        }

        if (_current->right_child != _nil) {
            // Case 1: 有右孩子。后继是右子树中的最小节点。
            _current = _find_minimum(_current->right_child);
        } else {
            // Case 2: 没有右孩子。向上找到第一个比当前节点大的祖先。
            const Rred_black_tree_node<E>* parent_node = _current->parent;
            while (parent_node != _nil && _current == parent_node->right_child) {
                _current = parent_node;
                parent_node = parent_node->parent;
            }
            // 循环结束时，parent_node 即为后继（或 nil）
            _current = parent_node; 
        }
        return *this;
    }

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_const_iterator Rred_black_tree<E>::Rred_black_tree_const_iterator::operator++(int) {
        // 后置递增
        Rred_black_tree_const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    // ==========================================================
    // Rred_black_tree_iterator 实现
    // ==========================================================

    template<typename E>
    Rred_black_tree<E>::Rred_black_tree_iterator::Rred_black_tree_iterator(Rred_black_tree_node<E>* node, Rred_black_tree_node<E>* nil_node) 
        : Rred_black_tree_const_iterator(node, nil_node) {}

    template<typename E>
    E& Rred_black_tree<E>::Rred_black_tree_iterator::operator*() const {
        // 解引用 (非 const 版本)
        // 将基类的 const 指针转换为非 const，以提供修改权限
        return const_cast<Rred_black_tree_node<E>*>(this->_current)->data;
    }

    template<typename E>
    E* Rred_black_tree<E>::Rred_black_tree_iterator::operator->() const {
        // 成员访问 (非 const 版本)
        return &(operator*());
    }

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_iterator& Rred_black_tree<E>::Rred_black_tree_iterator::operator++() {
        // 前置递增 (重用基类的逻辑)
        this->Rred_black_tree_const_iterator::operator++();
        return *this;
    }

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_iterator Rred_black_tree<E>::Rred_black_tree_iterator::operator++(int) {
        // 后置递增
        Rred_black_tree_iterator temp = *this;
        this->Rred_black_tree_const_iterator::operator++();
        return temp;
    }

    // ==========================================================
    // Rred_black_tree 迭代器接口实现
    // ==========================================================

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_iterator Rred_black_tree<E>::begin() {
        if (this->root == this->nil) {
            return Rred_black_tree_iterator(this->nil, this->nil);
        }
        // 调用非 const _tree_minimum
        return Rred_black_tree_iterator(_tree_minimum(this->root), this->nil);
    }

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_iterator Rred_black_tree<E>::end() {
        return Rred_black_tree_iterator(this->nil, this->nil);
    }

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_const_iterator Rred_black_tree<E>::begin() const {
        if (this->root == this->nil) {
            return Rred_black_tree_const_iterator(this->nil, this->nil);
        }
        // 调用 const _tree_minimum
        return Rred_black_tree_const_iterator(_tree_minimum(this->root), this->nil);
    }

    template<typename E>
    typename Rred_black_tree<E>::Rred_black_tree_const_iterator Rred_black_tree<E>::end() const {
        return Rred_black_tree_const_iterator(this->nil, this->nil);
    }
}

#endif //RRED_BLACK_TREE_TPP