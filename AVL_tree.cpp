#include <algorithm>
#include <iostream>
// Copyright by Rosetta
/* AVL node */
template <class T>
class AVLnode {
public:
    T key;
    int balance;
    AVLnode *left, *right, *parent;
 
    AVLnode(T k, AVLnode *p) : key(k), balance(0), parent(p), left(nullptr), right(nullptr) {}
 
    ~AVLnode() {
        delete left;
        delete right;
    }
};
 
/* AVL tree */
template <class T>
class AVLtree {
public:
    AVLtree(void);
    ~AVLtree(void);
    void insert(T key);
    void deleteKey(const T key);
    void print();
 
private:
    AVLnode<T> *root;
 
    AVLnode<T>* rotateLeft          ( AVLnode<T> *a );
    AVLnode<T>* rotateRight         ( AVLnode<T> *a );
    AVLnode<T>* rotateLeftThenRight ( AVLnode<T> *a );
    AVLnode<T>* rotateRightThenLeft ( AVLnode<T> *n );
    void rebalance                  ( AVLnode<T> *n );
    int height                     ( AVLnode<T> *n );
    void setBalance                 ( AVLnode<T> *n );
    void printBalance               ( AVLnode<T> *n );
    void clearNode                  ( AVLnode<T> *n );
};
 
/* AVL class definition */
template <class T>
void AVLtree<T>::rebalance(AVLnode<T> *n) {
    setBalance(n);
 
    if (n->balance == -2) {
        if (height(n->left->left) >= height(n->left->right))
            n = rotateRight(n);
        else
            n = rotateLeftThenRight(n);
    }
    else if (n->balance == 2) {
        if (height(n->right->right) >= height(n->right->left))
            n = rotateLeft(n);
        else
            n = rotateRightThenLeft(n);
    }
 
    if (n->parent != nullptr) {
        rebalance(n->parent);
    }
    else {
        root = n;
    }
}
 
template <class T>
AVLnode<T>* AVLtree<T>::rotateLeft(AVLnode<T> *a) {
    AVLnode<T> *b = a->right;
    b->parent = a->parent;
    a->right = b->left;
 
    if (a->right != nullptr)
        a->right->parent = a;
 
    b->left = a;
    a->parent = b;
 
    if (b->parent != nullptr) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }
    setBalance(a);
    setBalance(b);
    return b;
}
 
template <class T>
AVLnode<T>* AVLtree<T>::rotateRight(AVLnode<T> *a) {
    AVLnode<T> *b = a->left;
    b->parent = a->parent;
    a->left = b->right;
 
    if (a->left != nullptr)
        a->left->parent = a;
 
    b->right = a;
    a->parent = b;
 
    if (b->parent != nullptr) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }
 
    setBalance(a);
    setBalance(b);
    return b;
}
 
template <class T>
AVLnode<T>* AVLtree<T>::rotateLeftThenRight(AVLnode<T> *n) {
    n->left = rotateLeft(n->left);
    return rotateRight(n);
}
 
template <class T>
AVLnode<T>* AVLtree<T>::rotateRightThenLeft(AVLnode<T> *n) {
    n->right = rotateRight(n->right);
    return rotateLeft(n);
}
 
template <class T>
int AVLtree<T>::height(AVLnode<T> *n) {
    if (n == nullptr)
        return 0;
    return 1 + std::max(height(n->left), height(n->right));
}
 
template <class T>
void AVLtree<T>::setBalance(AVLnode<T> *n) {
    n->balance = height(n->right) - height(n->left);
}
 
template <class T>
void AVLtree<T>::printBalance(AVLnode<T> *n) {
    if (n != nullptr) {
        printBalance(n->left);
        std::cout << n->balance << " ";
        printBalance(n->right);
    }
}
 
template <class T>
AVLtree<T>::AVLtree(void) : root(nullptr) {}
 
template <class T>
AVLtree<T>::~AVLtree(void) {
    delete root;
}
 
template <class T>
void AVLtree<T>::insert(T key) {
    if (root == nullptr) {
        root = new AVLnode<T>(key, nullptr);
    }
    else {
        AVLnode<T>
            *n = root, 
            *parent = nullptr;
 
        while (true) {
            if (n->key == key)
                return;
 
            parent = n;
 
            bool goLeft = n->key > key;
            n = goLeft ? n->left : n->right;
 
            if (n == nullptr) {
                if (goLeft) {
                    parent->left = new AVLnode<T>(key, parent);
                }
                else {
                    parent->right = new AVLnode<T>(key, parent);
                }
 
                rebalance(parent);
                break;
            }
        }
    }
 
    return;
}
 
template <class T>
void AVLtree<T>::deleteKey(const T delKey) {
    if (root == nullptr)
        return;
 
    AVLnode<T>
        *n       = root,
        *parent  = root,
        *delNode = nullptr,
        *child   = root;
 
    while (child != nullptr) {
        parent = n;
        n = child;
        
        child = delKey >= n->key ? n->right : n->left;
        
        if (delKey == n->key){
            delNode = n;
            child = delNode->right != nullptr ? n->right : n->left;
        }
    }
 
    if (delNode != nullptr) {
        if(delNode == n){
            clearNode(n);
            return;
        }

        delNode->key = n->key;
        
        child = n->left == nullptr ? n->right : n->left;

        if (parent->left == n) {
            parent->left = child;
        }
        else {
            parent->right = child;
        }
        
        if(child != nullptr)
            child -> parent = parent;

        clearNode(n);

        rebalance(parent);        
    }
}

template <class T>
void AVLtree<T>::clearNode( AVLnode<T> *n ){
    if(n == root)
        root = nullptr;
    delete n;
}

template <class T>
void AVLtree<T>::print() {
    printBalance(root);
    std::cout << std::endl;
}
 
int main(void)
{
    AVLtree<int> t;
 
    std::cout << "Inserting integer values 1 to n" << std::endl;

    for (int i = 1; i <= 100; ++i)
        t.insert(i);
 
    std::cout << "Printing balance: ";
    t.print();
}
