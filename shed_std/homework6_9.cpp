#include <iostream>
#include <string>
using namespace std;

// 二叉链表的节点结构（核心：数据+左、右指针）
typedef struct BiTNode {
    char data;               // 数据域
    struct BiTNode* lchild;  // 左孩子指针（链表的左链）
    struct BiTNode* rchild;  // 右孩子指针（链表的右链）
} BiTNode, *BiTree;          // BiTree为指向节点的指针类型（链表头指针概念）

// 初始化二叉链表（创建示例树：a为根，b左子树含d，c右子树含e、f）
void CreateBiTree(BiTree& T) {
    // 根节点
    T = new BiTNode;
    T->data = 'a';
    T->lchild = nullptr;
    T->rchild = nullptr;

    // 左子树：a->b->d
    T->lchild = new BiTNode;
    T->lchild->data = 'b';
    T->lchild->rchild = new BiTNode;
    T->lchild->rchild->data = 'd';
    T->lchild->rchild->lchild = nullptr;
    T->lchild->rchild->rchild = nullptr;
    T->lchild->lchild = nullptr;  // b的右孩子为空

    // 右子树：a->c->e、c->f
    T->rchild = new BiTNode;
    T->rchild->data = 'c';
    T->rchild->lchild = new BiTNode;
    T->rchild->lchild->data = 'e';
    T->rchild->lchild->lchild = nullptr;
    T->rchild->lchild->rchild = nullptr;
    T->rchild->rchild = new BiTNode;
 
    T->rchild->rchild->lchild = nullptr;
    T->rchild->rchild->rchild = nullptr;
}

// 递归打印二叉链表的树状结构（按层级缩进）
void PrintBiTree(BiTree T, int level) {
    if(T==nullptr) return;
    PrintBiTree(T->rchild,level+1);
    cout<<string(level*2,' ')<<T->data<<endl;
    PrintBiTree(T->lchild,level+1);
}

// 销毁二叉链表（释放内存）
void DestroyBiTree(BiTree& T) {
    if (T != nullptr) {
        DestroyBiTree(T->lchild);  // 销毁左链
        DestroyBiTree(T->rchild);  // 销毁右链
        delete T;                  // 释放当前节点
        T = nullptr;
    }
}

int main() {
    BiTree T = nullptr;  // 定义二叉链表的根指针
    CreateBiTree(T);     // 构建二叉链表
    cout << "二叉链表的树状结构：" << endl;
    PrintBiTree(T, 0);   // 打印结构
    DestroyBiTree(T);    // 销毁链表，防止内存泄漏
    return 0;
}