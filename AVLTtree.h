#ifndef MAIN_CPP_AVLTTREE_H
#define MAIN_CPP_AVLTTREE_H

#include "wet1util.h"
#include <iostream>
#include <memory>
template<class T>
class AVLNode {
    T value;
    AVLNode<T> *left_son;
    AVLNode<T> *right_son;
    AVLNode<T> *parent;
    int height;

public:

    AVLNode() : value(nullptr), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0) {}

    explicit AVLNode(const T &value) : value(value), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0){}

    ~AVLNode();

    T getValue() const { return value; }

    void setValue(const T &new_value);

    void setLeft(AVLNode<T> *left) { left_son = left; }

    AVLNode<T> *getLeft() const { return left_son; }

    void setRight(AVLNode<T> *right) { right_son = right; }

    AVLNode<T> *getRight() const { return right_son; }

    void setParent(AVLNode<T> *parent_) { parent = parent_; }

    AVLNode<T> *GetParent() const { return parent; }

    void setHeight(int new_height) { this->height = new_height; }

    int GetHeight() const { return this->height; }

    void UpdateHeight();

    int getBF() const;

    void tree_to_arr(T *array, int *location);

};

template<class T>
class AVLTree {

    AVLNode<T> *root;

    int (*compare)(const T &a, const T &b);

    void LeftRotation(AVLNode<T> *node);

    void RightRotation(AVLNode<T> *B);

    void InsertAux(AVLNode<T> *node, AVLNode<T> *new_node);


    void chekRotate(AVLNode<T> *node);

public:

    explicit AVLTree(int (*comp)(const T &a, const T &b)) : root(nullptr), compare(comp) {}

    ~AVLTree();//TODO- change to correct destructure

    AVLNode<T> *RemoveBinary(AVLNode<T> *to_delete, const T& value);

    StatusType Insert(const T &value);

    bool Remove(const T &value);

    bool empty();

    AVLNode<T> *findVal(const T &value) const;

    AVLNode<T> *findValAux(AVLNode<T> *node, const T &value) const;

    AVLNode<T> *FindDad(const T &value) const;

    AVLNode<T> *FindDadAux(AVLNode<T> *node, const T &value) const;


    void merge(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const;

    AVLNode<T> *GetRoot();


    static AVLNode<T> *arr_to_tree(T *array, int start, int end, int *to_fill);

    static AVLTree<T> *arr_to_tree(T *array, int start, int size, int (*comp)(const T &, const T &));

    AVLNode<T> *findMin(AVLNode<T> *node);

    AVLNode<T> *findMax(AVLNode<T> *node);

    int GetSizeAux(AVLNode<T> *node);

    void tree_to_arr(T *array);

};

template<class T>
void AVLNode<T>::tree_to_arr(T *array, int *location) {
    if (this->getLeft() != nullptr)
        this->getLeft()->tree_to_arr(array, location);
    array[*location] = this->getValue();
    *location = *location + 1;
    if (this->getRight() != nullptr)
        this->getRight()->tree_to_arr(array, location);
}

template<class T>
void AVLNode<T>::UpdateHeight() {
    int left_height = 0;
    int right_height = 0;

    if (right_son == nullptr) {
        right_height = -1;
    } else {
        right_height = right_son->GetHeight();
    }

    if (left_son == nullptr) {
        left_height = -1;
    } else {
        left_height = left_son->GetHeight();
    }

    this->height = (std::max(left_height, right_height) + 1);
}


template<class T>
int AVLNode<T>::getBF() const {
    int left_height, right_height;
    if (left_son == nullptr) {
        left_height = -1;
    } else {
        left_height = left_son->GetHeight();
    }
    if (right_son == nullptr) {
        right_height = -1;
    } else {
        right_height = right_son->GetHeight();
    }
    return left_height - right_height;
}

template<class T>
AVLNode<T>::~AVLNode() {
    //delete value;
    if(left_son != nullptr){
        delete left_son;
    }
    if(right_son != nullptr) {
        delete right_son;
    }
}

template<class T>
void AVLNode<T>::setValue(const T &new_value) {
    this->value = new_value;
}

// basic rotations
template<class T>
void AVLTree<T>::LeftRotation(AVLNode<T> *node) {
    if (node == nullptr) { return; }

    AVLNode<T> *temp_node = node->getRight();


    AVLNode<T> *temp_left = temp_node->getLeft();
    node->setRight(temp_left);
    //if(new_root->getRight() != NULL) {new_root->getRight()->setParent(root);}
    temp_node->setLeft(node);

    if (node->GetParent() == nullptr) {
        this->root = temp_node;
    } else {
        if (node->GetParent()->getLeft() == node) {
            node->GetParent()->setLeft(temp_node);
        } else {
            node->GetParent()->setRight(temp_node);
        }
    }
    temp_node->setParent(node->GetParent());
    node->setParent(temp_node);

    if (temp_left != nullptr) {
        temp_left->setParent(node);
    }
    node->UpdateHeight();
    temp_node->UpdateHeight();
}

template<class T>
void AVLTree<T>::RightRotation(AVLNode<T> *node) {
    if (node == nullptr) { return; }

    AVLNode<T> *temp_node = node->getLeft();


    AVLNode<T> *temp_right = temp_node->getRight();
    node->setLeft(temp_right);
    if (temp_right != nullptr) {
        temp_right->setParent(node);
    }
    //if(new_root->getRight() != NULL) {new_root->getRight()->setParent(root);}
    temp_node->setRight(node);

    if (node->GetParent() == nullptr) {
        this->root = temp_node;
    } else {
        if (node->GetParent()->getLeft() == node) {
            node->GetParent()->setLeft(temp_node);
        } else {
            node->GetParent()->setRight(temp_node);
        }
    }
    temp_node->setParent(node->GetParent());
    node->setParent(temp_node);

    node->UpdateHeight();
    temp_node->UpdateHeight();
}


//we return true if the insert succeeded
template<class T>
StatusType AVLTree<T>::Insert(const T &value) {

    //check if this value already exists

    if (this->findVal(value)) {
        return StatusType::FAILURE;
    }

    try{
        auto *new_node = new AVLNode<T>(value);

        //if our tree is empty
        if (this->root == nullptr) {
            this->root = new_node;
        } else {
            InsertAux(this->root, new_node);
        }

        return StatusType::SUCCESS;
    }
    catch (const std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }

}


template<class T>
void AVLTree<T>::InsertAux(AVLNode<T> *node, AVLNode<T> *new_node) {
    if (compare(node->getValue(), new_node->getValue()) == -1) {

        //if we don't have a left child
        if (node->getLeft() == nullptr) {
            node->setLeft(new_node);
            new_node->setParent(node);
        } else { //we need to keep looking in the left tree
            InsertAux(node->getLeft(), new_node);
        }
    } else {
        if (node->getRight() == nullptr) {
            node->setRight(new_node);
            new_node->setParent(node);
        } else {
            InsertAux(node->getRight(), new_node);
        }
    }
    //the function updates the node according to the rotates that are needed
    chekRotate(node);

//we need to update the height
}


//find smallest value in tree
template<class T>
AVLNode<T> *AVLTree<T>::findMin(AVLNode<T> *node) {
    if (node == nullptr)
        return node;

    if (node->getLeft() == nullptr)
        return node;
    return findMin(node->getLeft());
}


template <class T>
AVLNode<T>* AVLTree<T>::RemoveBinary(AVLNode<T>* node, const T& value)
{
    AVLNode<T>* to_delete = findVal(value);
    //start with the greatest if shrink the next conditions
    AVLNode<T>* big_parent = nullptr;

    //if to_delete has two sons
    if (to_delete->getLeft() && to_delete->getRight()) {
        //here can be minVal
        AVLNode<T>* new_root = findMin(to_delete->getRight());

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
    AVLNode<T>* parent = to_delete->GetParent();
    if (parent == nullptr) {
        root = big_parent;
    } else if (to_delete->GetParent()->getLeft() == to_delete) {
        to_delete->GetParent()->setLeft(big_parent);
    } else {
        to_delete->GetParent()->setRight(big_parent);
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
void AVLTree<T>::chekRotate(AVLNode<T> *node) {
    int balance_root = node->getBF();

    //if an LL is needed
    if (balance_root == 2 && (node->getLeft())->getBF() >= 0) {
        RightRotation(node);
    //if an LR is needed
    } else if (balance_root == 2 && (node->getLeft())->getBF() == -1) {
        LeftRotation(node->getLeft());
        RightRotation(node);
        return;
    //if an RL is needed
    } else if (balance_root == -2 && (node->getRight())->getBF() == 1) {
        RightRotation(node->getRight());
        LeftRotation(node);
        return;
    //if an RR is needed
    } else if (balance_root == -2 && (node->getRight())->getBF() <= 0) {
        LeftRotation(node);
        return;
    }

    node->UpdateHeight();
}


template<class T>
bool AVLTree<T>::Remove(const T &value) {
    if (root == nullptr)//if root is empty
        return false;


    AVLNode<T> *to_delete = this->findVal(value);
    if (to_delete == nullptr)//if value needed to delete doesn't exist
    {
        return false;
    }

    //regular binary tree remove
    AVLNode<T> *parent_of_deleted = RemoveBinary(root, value);

    while (parent_of_deleted != nullptr) {
        chekRotate(parent_of_deleted);
        parent_of_deleted = parent_of_deleted->GetParent();
    }
    return true;

}

template<class T>
int AVLTree<T>::GetSizeAux(AVLNode<T> *node) {
    if (node == nullptr)
        return 0;

    return 1 + GetSizeAux(node->getLeft()) + GetSizeAux(node->getRight());
}

template<class T>
void AVLTree<T>::tree_to_arr(T *array) {
    int location = 0;
    if (root != nullptr)
        root->tree_to_arr(array, &location);
}

template<class T>
AVLTree<T> *AVLTree<T>::arr_to_tree(T *array, int start, int size, int (*comp)(const T &a, const T &b)) {
    auto tree = new AVLTree<T>(comp);
    int to_fill = size;
    tree->root = arr_to_tree(array, start, size, &to_fill);
    //tree->Validate();
    return tree;
}

template <class T>
AVLNode<T> *AVLTree<T>::arr_to_tree(T *array, int start, int end, int* to_fill) {

    if (*to_fill ==0 || start >end)
    {
        return nullptr;
    }

    int mini_root_index = (start+end)/2;

    auto* mini_root = new AVLNode<T>(array[mini_root_index]);
    mini_root->setParent(nullptr);
    *to_fill = *to_fill -1;

    if (start-end == 0)
    {
        mini_root->setLeft(nullptr);
        mini_root->setRight(nullptr);
        mini_root->setHeight(0);
        return mini_root;
    }


    AVLNode<T>* left_son = arr_to_tree(array,start,mini_root_index-1,to_fill);

    AVLNode<T>* right_son = arr_to_tree(array,mini_root_index+1,end,to_fill);



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

    mini_root->UpdateHeight();
    return mini_root;

}


template<class T>
AVLNode<T> *AVLTree<T>::findVal(const T &value) const {
    AVLNode<T> *tmp = this->root;
    return findValAux(tmp, value);
}

template<class T>
AVLNode<T> *AVLTree<T>::findValAux(AVLNode<T> *node, const T &value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (compare(node->getValue(), value) == 0) {
        return node;
    }
    if (compare(value, node->getValue()) == -1) {
        return findValAux(node->getRight(), value);
    } else {
        return findValAux(node->getLeft(), value);
    }

}

template<class T>
AVLNode<T> *AVLTree<T>::FindDad(const T &value) const {
    AVLNode<T> *tmp = this->root;
    if(this->root== nullptr) {
        return nullptr;
    }
    return FindDadAux(tmp, value);
}

template<class T>
AVLNode<T> *AVLTree<T>::FindDadAux(AVLNode<T> *node, const T &value) const {
    if (compare(value, node->getValue()) == -1) {
        if(node -> getRight() == nullptr){
            return node;
        }
        return FindDadAux(node->getRight(), value);
    } else {
        if(node -> getLeft() == nullptr){
            return node;
        }
        return FindDadAux(node->getLeft(), value);
    }

}

template<class T>
bool AVLTree<T>::empty() {
    return this->root == nullptr;
}

template<class T>
AVLNode<T> *AVLTree<T>::GetRoot() {
    return this->root;
}

template<class T>
AVLNode<T> *AVLTree<T>::findMax(AVLNode<T> *node) {
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


