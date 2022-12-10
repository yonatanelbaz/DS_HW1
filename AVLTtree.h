#ifndef MAIN_CPP_AVLTTREE_H
#define MAIN_CPP_AVLTTREE_H

#include "wet1util.h"
#include <iostream>
#include <memory>
template<class T>
class Node {
    T value;
    Node<T> *left_son;
    Node<T> *right_son;
    Node<T> *parent;
    int height;

public:

    Node() : value(nullptr), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0) {}

    explicit Node(const T &value) : value(value), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0){}

    ~Node();

    T getValue() const { return value; }

    void setValue(const T &new_value);

    void setLeft(Node<T> *left) { left_son = left; }

    Node<T> *getLeft() const { return left_son; }

    void setRight(Node<T> *right) { right_son = right; }

    Node<T> *getRight() const { return right_son; }

    void setParent(Node<T> *parent_) { parent = parent_; }

    Node<T> *getParent() const { return parent; }

    void setHeight(int new_height) { this->height = new_height; }

    int getHeight() const { return this->height; }

    void updateHeight();

    int calcBalanceFactor() const;

    void convertTreeToArr(T *array, int *location);

};

template<class T>
class AVLTree {

    Node<T> *root;

    int (*compare)(const T &a, const T &b);

    void leftRotation(Node<T> *node);

    void rightRotation(Node<T> *B);

    void insertRec(Node<T> *node, Node<T> *new_node);


    void checkRotate(Node<T> *node);

public:
    explicit AVLTree(int (*comp)(const T &a, const T &b)) : root(nullptr), compare(comp) {}
    ~AVLTree();//TODO- change to correct destructure
    Node<T> *removeNodeNoRotations(Node<T> *to_delete, const T& value);
    StatusType insert(const T &value);
    Node<T> *findParentRec(Node<T> *node, const T &value) const;
    bool remove(const T &value);
    Node<T> *findParent(const T &value) const;
    bool empty();
    Node<T> *findVal(const T &value) const;
    void merge(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const;
    Node<T> *getRoot();
    Node<T> *findValRec(Node<T> *node, const T &value) const;
    static Node<T> *covertArrToTree(T *array, int start, int end, int *to_fill);
    void convertTreeToArr(T *array);
    static AVLTree<T> *covertArrToTree(T *array, int start, int size, int (*comp)(const T &, const T &));
    Node<T> *findMin(Node<T> *node);
    Node<T> *findMax(Node<T> *node);
    int getSizeRec(Node<T> *node);
};

template<class T>
void Node<T>::convertTreeToArr(T *array, int *location) {
    if (this->getLeft() != nullptr)
        this->getLeft()->convertTreeToArr(array, location);
    array[*location] = this->getValue();
    *location = *location + 1;
    if (this->getRight() != nullptr)
        this->getRight()->convertTreeToArr(array, location);
}

template<class T>
void Node<T>::updateHeight() {
    int left_height = 0;
    int right_height = 0;

    if (right_son == nullptr) {
        right_height = -1;
    } else {
        right_height = right_son->getHeight();
    }

    if (left_son == nullptr) {
        left_height = -1;
    } else {
        left_height = left_son->getHeight();
    }

    this->height = (std::max(left_height, right_height) + 1);
}


template<class T>
int Node<T>::calcBalanceFactor() const {
    int left_height, right_height;
    if (left_son == nullptr) {
        left_height = -1;
    } else {
        left_height = left_son->getHeight();
    }
    if (right_son == nullptr) {
        right_height = -1;
    } else {
        right_height = right_son->getHeight();
    }
    return left_height - right_height;
}

template<class T>
Node<T>::~Node() {
    //delete value;
    if(left_son != nullptr){
        delete left_son;
    }
    if(right_son != nullptr) {
        delete right_son;
    }
}

template<class T>
void Node<T>::setValue(const T &new_value) {
    this->value = new_value;
}

// basic rotations
template<class T>
void AVLTree<T>::leftRotation(Node<T> *node) {
    if (node == nullptr) { return; }

    Node<T> *temp_node = node->getRight();


    Node<T> *temp_left = temp_node->getLeft();
    node->setRight(temp_left);
    //if(new_root->getRight() != NULL) {new_root->getRight()->setParent(root);}
    temp_node->setLeft(node);

    if (node->getParent() == nullptr) {
        this->root = temp_node;
    } else {
        if (node->getParent()->getLeft() == node) {
            node->getParent()->setLeft(temp_node);
        } else {
            node->getParent()->setRight(temp_node);
        }
    }
    temp_node->setParent(node->getParent());
    node->setParent(temp_node);

    if (temp_left != nullptr) {
        temp_left->setParent(node);
    }
    node->updateHeight();
    temp_node->updateHeight();
}

template<class T>
void AVLTree<T>::rightRotation(Node<T> *node) {
    if (node == nullptr) { return; }

    Node<T> *temp_node = node->getLeft();


    Node<T> *temp_right = temp_node->getRight();
    node->setLeft(temp_right);
    if (temp_right != nullptr) {
        temp_right->setParent(node);
    }
    //if(new_root->getRight() != NULL) {new_root->getRight()->setParent(root);}
    temp_node->setRight(node);

    if (node->getParent() == nullptr) {
        this->root = temp_node;
    } else {
        if (node->getParent()->getLeft() == node) {
            node->getParent()->setLeft(temp_node);
        } else {
            node->getParent()->setRight(temp_node);
        }
    }
    temp_node->setParent(node->getParent());
    node->setParent(temp_node);

    node->updateHeight();
    temp_node->updateHeight();
}


//we return true if the insert succeeded
template<class T>
StatusType AVLTree<T>::insert(const T &value) {

    //check if this value already exists

    if (this->findVal(value)) {
        return StatusType::FAILURE;
    }

    try{
        auto *new_node = new Node<T>(value);

        //if our tree is empty
        if (this->root == nullptr) {
            this->root = new_node;
        } else {
            insertRec(this->root, new_node);
        }

        return StatusType::SUCCESS;
    }
    catch (const std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }

}


template<class T>
void AVLTree<T>::insertRec(Node<T> *node, Node<T> *new_node)  {
    if (compare(node->getValue(), new_node->getValue()) == -1) {

        //if we don't have a left child
        if (node->getLeft() == nullptr) {
            node->setLeft(new_node);
            new_node->setParent(node);
        } else { //we need to keep looking in the left tree
            insertRec(node->getLeft(), new_node);
        }
    } else {
        if (node->getRight() == nullptr) {
            node->setRight(new_node);
            new_node->setParent(node);
        } else {
            insertRec(node->getRight(), new_node);
        }
    }
    //the function updates the node according to the rotates that are needed
    checkRotate(node);

//we need to update the height
}


//find smallest value in tree
template<class T>
Node<T> *AVLTree<T>::findMin(Node<T> *node) {
    if (node == nullptr)
        return node;

    if (node->getLeft() == nullptr)
        return node;
    return findMin(node->getLeft());
}


template <class T>
Node<T>* AVLTree<T>::removeNodeNoRotations(Node<T>* node, const T& value)
{
    Node<T>* to_delete = findVal(value);
    //start with the greatest if shrink the next conditions
    Node<T>* big_parent = nullptr;

    //if to_delete has two sons
    if (to_delete->getLeft() && to_delete->getRight()) {
        //here can be minVal
        Node<T>* new_root = findMin(to_delete->getRight());

        //switching the value is soooo smart !!!
        to_delete->setValue(new_root->getValue());
        to_delete = new_root;
        if (to_delete->getRight()) {
            big_parent = to_delete->getRight();
        }
    }

    else if (to_delete->getLeft()) {
        big_parent = to_delete->getLeft();
    } else if (to_delete->getRight()) {
        big_parent = to_delete->getRight();
    }

    //All cases had the same below logic in the end - set new child to the parent
    Node<T>* parent = to_delete->getParent();
    if (parent == nullptr) {
        root = big_parent;
    } else if (to_delete->getParent()->getLeft() == to_delete) {
        to_delete->getParent()->setLeft(big_parent);
    } else {
        to_delete->getParent()->setRight(big_parent);
    }
    if (big_parent != nullptr) {
        big_parent->setParent(parent);
    }

    //deleting the node
    to_delete->setLeft(nullptr);
    to_delete->setRight(nullptr);
    delete to_delete;
    return parent;
}


//TODO - change all things
template<class T>
void AVLTree<T>::checkRotate(Node<T> *node)  {
    int balance_root = node->calcBalanceFactor();

    //if an LL is needed
    if (balance_root == 2 && (node->getLeft())->calcBalanceFactor() >= 0) {
        rightRotation(node);
    //if an LR is needed
    } else if (balance_root == 2 && (node->getLeft())->calcBalanceFactor() == -1) {
        leftRotation(node->getLeft());
        rightRotation(node);
        return;
    //if an RL is needed
    } else if (balance_root == -2 && (node->getRight())->calcBalanceFactor() == 1) {
        rightRotation(node->getRight());
        leftRotation(node);
        return;
    //if an RR is needed
    } else if (balance_root == -2 && (node->getRight())->calcBalanceFactor() <= 0) {
        leftRotation(node);
        return;
    }

    node->updateHeight();
}


template<class T>
bool AVLTree<T>::remove(const T &value) {
    if (root == nullptr)//if root is empty
        return false;


    Node<T> *to_delete = this->findVal(value);
    if (to_delete == nullptr)//if value needed to delete doesn't exist
    {
        return false;
    }

    //regular binary tree remove
    Node<T> *parent_of_deleted = removeNodeNoRotations(root, value);

    while (parent_of_deleted != nullptr) {
        checkRotate(parent_of_deleted);
        parent_of_deleted = parent_of_deleted->getParent();
    }
    return true;

}

template<class T>
int AVLTree<T>::getSizeRec(Node<T> *node) {
    if (node == nullptr)
        return 0;

    return 1 + getSizeRec(node->getLeft()) + getSizeRec(node->getRight());
}

template<class T>
void AVLTree<T>::convertTreeToArr(T *array) {
    int location = 0;
    if (root != nullptr)
        root->convertTreeToArr(array, &location);
}

template<class T>
AVLTree<T> *AVLTree<T>::covertArrToTree(T *array, int start, int size, int (*comp)(const T &a, const T &b)) {
    auto tree = new AVLTree<T>(comp);
    int to_fill = size;
    tree->root = covertArrToTree(array, start, size, &to_fill);
    //tree->Validate();
    return tree;
}

template <class T>
Node<T> *AVLTree<T>::covertArrToTree(T *array, int start, int end, int* to_fill) {

    if (*to_fill ==0 || start >end)
    {
        return nullptr;
    }

    int mini_root_index = (start+end)/2;

    auto* mini_root = new Node<T>(array[mini_root_index]);
    mini_root->setParent(nullptr);
    *to_fill = *to_fill -1;

    if (start-end == 0)
    {
        mini_root->setLeft(nullptr);
        mini_root->setRight(nullptr);
        mini_root->setHeight(0);
        return mini_root;
    }


    Node<T>* left_son = covertArrToTree(array,start,mini_root_index-1,to_fill);

    Node<T>* right_son = covertArrToTree(array,mini_root_index+1,end,to_fill);



    mini_root->setLeft(left_son);
    mini_root->setRight(right_son);
    if (left_son!= nullptr)
    {
        left_son->setParent(mini_root);
    }
    if (right_son != nullptr)
    {
        right_son->setParent(mini_root);
    }

    mini_root->updateHeight();
    return mini_root;

}


template<class T>
Node<T> *AVLTree<T>::findVal(const T &value) const {
    Node<T> *tmp = this->root;
    return findValRec(tmp, value);
}

template<class T>
Node<T> *AVLTree<T>::findValRec(Node<T> *node, const T &value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (compare(node->getValue(), value) == 0) {
        return node;
    }
    if (compare(value, node->getValue()) == -1) {
        return findValRec(node->getRight(), value);
    } else {
        return findValRec(node->getLeft(), value);
    }

}

template<class T>
Node<T> *AVLTree<T>::findParent(const T &value) const {
    Node<T> *tmp = this->root;
    if(this->root== nullptr) {
        return nullptr;
    }
    return findParentRec(tmp, value);
}

template<class T>
Node<T> *AVLTree<T>::findParentRec(Node<T> *node, const T &value) const {
    if (compare(value, node->getValue()) == -1) {
        if(node -> getRight() == nullptr){
            return node;
        }
        return findParentRec(node->getRight(), value);
    } else {
        if(node -> getLeft() == nullptr){
            return node;
        }
        return findParentRec(node->getLeft(), value);
    }

}

template<class T>
bool AVLTree<T>::empty() {
    return this->root == nullptr;
}

template<class T>
Node<T> *AVLTree<T>::getRoot() {
    return this->root;
}

template<class T>
Node<T> *AVLTree<T>::findMax(Node<T> *node) {
    if(node->getValue()== nullptr) {
        return nullptr;
    }
    if (node->getRight() == nullptr)
        return node;
    return findMax(node->getRight());
}


template<class T>
AVLTree<T>::~AVLTree(){
    if(this->root != nullptr){
        delete this->root;
    }
    //root = nullptr;
}


template<class T>
void AVLTree<T>::merge(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const {

    while (arr1_size != 0 && arr2_size != 0) {
        if (compare(*arr1, *arr2) == 1) {
            *merge_to = *arr1;
            arr1 = arr1 + 1;
            arr1_size = arr1_size - 1;
            merge_to = merge_to + 1;
        } else {
            *merge_to = *arr2;
            arr2 = arr2 + 1;
            arr2_size = arr2_size - 1;
            merge_to = merge_to + 1;
        }
    }

    while (arr1_size != 0) {
        *merge_to = *arr1;
        arr1 = arr1 + 1;
        arr1_size = arr1_size - 1;
        merge_to = merge_to + 1;
    }

    while (arr2_size != 0) {
        *merge_to = *arr2;
        arr2 = arr2 + 1;
        arr2_size = arr2_size - 1;
        merge_to = merge_to + 1;
    }
}



#endif //MAIN_CPP_AVLTTREE_H


