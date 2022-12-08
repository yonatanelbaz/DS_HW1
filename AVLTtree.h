#ifndef MAIN_CPP_AVLTTREE_H
#define MAIN_CPP_AVLTTREE_H
//TODO DELETE
#include "wet1util.h"
#include <iostream>
#include <memory>
template<class T>
class AVLNode {
    T data;
    AVLNode<T> *left_son;
    AVLNode<T> *right_son;
    AVLNode<T> *parent;
    int height;
public:

    AVLNode() : data(nullptr), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0) {}

    explicit AVLNode(const T &value) : data(value), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0){}

    ~AVLNode();

    const AVLNode<T> *GetMinNode() const;

    T GetValue() const { return data; }

    void SetValue(const T &new_data);

    void SetLeft(AVLNode<T> *left) { left_son = left; }

    AVLNode<T> *GetLeft() const { return left_son; }

    void SetRight(AVLNode<T> *right) { right_son = right; }

    AVLNode<T> *GetRight() const { return right_son; }

    void SetParent(AVLNode<T> *parent_) { parent = parent_; }

    AVLNode<T> *GetParent() const { return parent; }

    void SetHeight(int new_height) { this->height = new_height; }

    int GetHeight() const { return this->height; }

    void UpdateHeight();

    int GetBalanceFactor() const;

    AVLNode<T> *GetGreateParent();

    AVLNode<T> *GetGreateParentAux(AVLNode<T> *node);

    void ToArray(T *array, int *location);

};

template<class T> //to search online!!!!!!!!!!
class AVLTree {

    AVLNode<T> *root;

    int (*compare)(const T &a, const T &b);
/*
 * compare return value:
 * 1 - a value is bigger than b
 * 0 - a value is equal than b
 * -1 - a value is smaller than b
 */
    void RotateLeft(AVLNode<T> *node);

    void RotateRight(AVLNode<T> *B);

    void InsertAux(AVLNode<T> *node, AVLNode<T> *new_node);


    void RotateByNeeded(AVLNode<T> *node);

public:

    explicit AVLTree(int (*comp)(const T &a, const T &b)) : root(nullptr), compare(comp) {}

    ~AVLTree();//TODO- change to correct destructure

    AVLNode<T> *RemoveBinary(AVLNode<T> *to_delete, const T& value);

    StatusType Insert(const T &data);

    bool Remove(const T &data);

    bool IsEmpty();

    const AVLNode<T> *GetMinNode() const;

    AVLNode<T> *Find(const T &value) const;

    AVLNode<T> *FindAux(AVLNode<T> *node, const T &value) const;

    AVLNode<T> *FindDad(const T &value) const;

    AVLNode<T> *FindDadAux(AVLNode<T> *node, const T &value) const;

    AVLNode<T> *GetSmallestInTree(); //TODO: check if we need to delete

    void merge(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const;

    AVLNode<T> *GetRoot();

    void SetRoot(AVLNode<T> *new_root);

    static AVLNode<T> *FromArrayToTree(T *array, int start, int end, int *to_fill);

    static AVLTree<T> *FromArrayToTree(T *array, int start, int size, int (*comp)(const T &, const T &));

    AVLNode<T> *FindMinValInTree(AVLNode<T> *node);

    AVLNode<T> *FindMaxValInTree(AVLNode<T> *node);

    int GetTreeSize();

    int GetSizeAux(AVLNode<T> *node);

    void ToArray(T *array);

};

template<class T>
void AVLNode<T>::ToArray(T *array, int *location) {
    if (this->GetLeft() != nullptr)
        this->GetLeft()->ToArray(array, location);
    array[*location] = this->GetValue();
    *location = *location + 1;
    if (this->GetRight() != nullptr)
        this->GetRight()->ToArray(array, location);
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
int AVLNode<T>::GetBalanceFactor() const {
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
AVLNode<T> *AVLNode<T>::GetGreateParent() {
    if (this->parent == nullptr)
        return this;
    return GetGreateParentAux(this->parent);
}

template<class T>
AVLNode<T> *AVLNode<T>::GetGreateParentAux(AVLNode<T> *node) {
    if (node->parent == nullptr)
        return node;
    return GetGreateParentAux(node->parent);
}

template<class T>
AVLNode<T>::~AVLNode() {
    //delete data;
   delete left_son;
    delete right_son;
    //this-> = nullptr;
}

template<class T>
void AVLNode<T>::SetValue(const T &new_data) {
    this->data = new_data;
}

template<class T>
const AVLNode<T> *AVLNode<T>::GetMinNode() const {
    if (GetLeft() == nullptr)
        return this;
    else
        return GetLeft()->GetMinNode();
}





// basic rotations
template<class T>
void AVLTree<T>::RotateLeft(AVLNode<T> *B) {
    if (B == nullptr) { return; }

    AVLNode<T> *A = B->GetRight();


    AVLNode<T> *A_L = A->GetLeft();
    B->SetRight(A_L);
    //if(new_root->GetRight() != NULL) {new_root->GetRight()->SetParent(root);}
    A->SetLeft(B);

    if (B->GetParent() == nullptr) {
        this->root = A;
    } else {
        if (B->GetParent()->GetLeft() == B) {
            B->GetParent()->SetLeft(A);
        } else {
            B->GetParent()->SetRight(A);
        }
    }
    A->SetParent(B->GetParent());
    B->SetParent(A);

    if (A_L != nullptr) {
        A_L->SetParent(B);
    }
    B->UpdateHeight();
    A->UpdateHeight();
}

template<class T>
void AVLTree<T>::RotateRight(AVLNode<T> *B) {
    if (B == nullptr) { return; }

    AVLNode<T> *A = B->GetLeft();


    AVLNode<T> *A_R = A->GetRight();
    B->SetLeft(A_R);
    if (A_R != nullptr) {
        A_R->SetParent(B);
    }
    //if(new_root->GetRight() != NULL) {new_root->GetRight()->SetParent(root);}
    A->SetRight(B);

    if (B->GetParent() == nullptr) {
        this->root = A;
    } else {
        if (B->GetParent()->GetLeft() == B) {
            B->GetParent()->SetLeft(A);
        } else {
            B->GetParent()->SetRight(A);
        }
    }
    A->SetParent(B->GetParent());
    B->SetParent(A);

    B->UpdateHeight();
    A->UpdateHeight();
}


//we return true if the insert succeeded
template<class T>
StatusType AVLTree<T>::Insert(const T &data) {

    //check if this value already exists

    if (this->Find(data)) {
        return StatusType::FAILURE;
    }

    try{
        auto *new_node = new AVLNode<T>(data);

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
    if (compare(node->GetValue(), new_node->GetValue()) == 1) {

        //if we don't have a left child
        if (node->GetLeft() == nullptr) {
            node->SetLeft(new_node);
            new_node->SetParent(node);
        } else { //we need to keep looking in the left tree
            InsertAux(node->GetLeft(), new_node);
        }
    } else {
        if (node->GetRight() == nullptr) {
            node->SetRight(new_node);
            new_node->SetParent(node);
        } else {
            InsertAux(node->GetRight(), new_node);
        }
    }
    //the function updates the node according to the rotates that are needed
    RotateByNeeded(node);

//we need to update the height
}


//find smallest value in tree
template<class T>
AVLNode<T> *AVLTree<T>::FindMinValInTree(AVLNode<T> *node) {
    if (node == nullptr)
        return node;

    if (node->GetLeft() == nullptr)
        return node;
    return FindMinValInTree(node->GetLeft());
}


template <class T>
AVLNode<T>* AVLTree<T>::RemoveBinary(AVLNode<T>* node, const T& data)
{
    AVLNode<T>* to_delete = Find(data);
    //start with the greatest if shrink the next conditions
    AVLNode<T>* big_parent = nullptr;

    //if to_delete has two sons
    if (to_delete->GetLeft() && to_delete->GetRight()) {
        //here can be minVal
        AVLNode<T>* new_root = FindMinValInTree(to_delete->GetRight());

        //switching the data is soooo smart !!!
        to_delete->SetValue(new_root->GetValue());
        to_delete = new_root;
        if (to_delete->GetRight()) {
            big_parent = to_delete->GetRight();
        }
    }

    else if (to_delete->GetLeft()) {
        big_parent = to_delete->GetLeft();
    } else if (to_delete->GetRight()) {
        big_parent = to_delete->GetRight();
    }

    //All cases had the same below logic in the end - set new child to the parent
    AVLNode<T>* parent = to_delete->GetParent();
    if (parent == nullptr) {
        root = big_parent;
    } else if (to_delete->GetParent()->GetLeft() == to_delete) {
        to_delete->GetParent()->SetLeft(big_parent);
    } else {
        to_delete->GetParent()->SetRight(big_parent);
    }
    if (big_parent != nullptr) {
        big_parent->SetParent(parent);
    }

    //deleting the node
    to_delete->SetLeft(nullptr);
    to_delete->SetRight(nullptr);
    delete to_delete;
    return parent;
}


//TODO - change all things
template<class T>
void AVLTree<T>::RotateByNeeded(AVLNode<T> *node) {
    int balance_root = node->GetBalanceFactor();

    //if an LL is needed
    if (balance_root == 2 && (node->GetLeft())->GetBalanceFactor() >= 0) {
        RotateRight(node);
    //if an LR is needed
    } else if (balance_root == 2 && (node->GetLeft())->GetBalanceFactor() == -1) {
        RotateLeft(node->GetLeft());
        RotateRight(node);
        return;
    //if an RL is needed
    } else if (balance_root == -2 && (node->GetRight())->GetBalanceFactor() == 1) {
        RotateRight(node->GetRight());
        RotateLeft(node);
        return;
    //if an RR is needed
    } else if (balance_root == -2 && (node->GetRight())->GetBalanceFactor() <= 0) {
        RotateLeft(node);
        return;
    }

    node->UpdateHeight();
}


template<class T>
bool AVLTree<T>::Remove(const T &data) {
    if (root == nullptr)//if root is empty
        return false;


    AVLNode<T> *to_delete = this->Find(data);
    if (to_delete == nullptr)//if data needed to delete doesn't exist
    {
        return false;
    }

    //regular binary tree remove
    AVLNode<T> *parent_of_deleted = RemoveBinary(root, data);

    while (parent_of_deleted != nullptr) {
        RotateByNeeded(parent_of_deleted);
        parent_of_deleted = parent_of_deleted->GetParent();
    }
    return true;

}


template<class T>
int AVLTree<T>::GetTreeSize() {
    return GetSizeAux(this->root);
}

template<class T>
int AVLTree<T>::GetSizeAux(AVLNode<T> *node) {
    if (node == nullptr)
        return 0;

    return 1 + GetSizeAux(node->GetLeft()) + GetSizeAux(node->GetRight());
}

template<class T>
void AVLTree<T>::ToArray(T *array) {
    int location = 0;
    if (root != nullptr)
        root->ToArray(array, &location);
}

template<class T>
AVLTree<T> *AVLTree<T>::FromArrayToTree(T *array, int start, int size, int (*comp)(const T &a, const T &b)) {
    auto tree = new AVLTree<T>(comp);
    int to_fill = size;
    tree->root = FromArrayToTree(array, start, size, &to_fill);
    //tree->Validate();
    return tree;
}

template <class T>
AVLNode<T> *AVLTree<T>::FromArrayToTree(T *array, int start, int end, int* to_fill) {

    if (*to_fill ==0 || start >end)
    {
        return nullptr;
    }

    int mini_root_index = (start+end)/2;

    auto* mini_root = new AVLNode<T>(array[mini_root_index]);
    mini_root->SetParent(nullptr);
    *to_fill = *to_fill -1;

    if (start-end == 0)
    {
        mini_root->SetLeft(nullptr);
        mini_root->SetRight(nullptr);
        mini_root->SetHeight(0);
        return mini_root;
    }


    AVLNode<T>* left_son = FromArrayToTree(array,start,mini_root_index-1,to_fill);

    AVLNode<T>* right_son = FromArrayToTree(array,mini_root_index+1,end,to_fill);



    mini_root->SetLeft(left_son);
    mini_root->SetRight(right_son);
    if (left_son!= nullptr)
    {
        left_son->SetParent(mini_root);
    }
    if (right_son != nullptr)
    {
        right_son->SetParent(mini_root);
    }

    mini_root->UpdateHeight();
    return mini_root;

}


template<class T>
AVLNode<T> *AVLTree<T>::Find(const T &value) const {
    AVLNode<T> *tmp = this->root;
    return FindAux(tmp, value);
}

template<class T>
AVLNode<T> *AVLTree<T>::FindAux(AVLNode<T> *node, const T &value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (compare(node->GetValue(), value) == 0) {
        return node;
    }
    if (compare(value, node->GetValue()) == 1) {
        return FindAux(node->GetRight(), value);
    } else {
        return FindAux(node->GetLeft(), value);
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
    if (compare(value, node->GetValue()) == 1) {
        if(node -> GetRight() == nullptr){
            return node;
        }
        return FindDadAux(node->GetRight(), value);
    } else {
        if(node -> GetLeft() == nullptr){
            return node;
        }
        return FindDadAux(node->GetLeft(), value);
    }

}

template<class T>
bool AVLTree<T>::IsEmpty() {
    return this->root == nullptr;
}

template<class T>
AVLNode<T> *AVLTree<T>::GetSmallestInTree() {
    if (root == nullptr)
        return nullptr;

    AVLNode<T> *smallest = root;
    while (smallest->GetLeft() != nullptr) {
        smallest = smallest->GetLeft();
    }
    return smallest;
}

template<class T>
AVLNode<T> *AVLTree<T>::GetRoot() {
    return this->root;
}

template<class T>
void AVLTree<T>::SetRoot(AVLNode<T> *new_root) {
    this->root = new_root;
}

template<class T>
AVLNode<T> *AVLTree<T>::FindMaxValInTree(AVLNode<T> *node) {
    if(node->GetValue()== nullptr) {
        return nullptr;
    }
    if (node->GetRight() == nullptr)
        return node;
    return FindMaxValInTree(node->GetRight());
}


template<class T>
AVLTree<T>::~AVLTree(){
    delete root;
    //root = nullptr;
}


template<class T>
void AVLTree<T>::merge(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const {

    while (arr1_size != 0 && arr2_size != 0) {
        if (compare(*arr1, *arr2) == -1) {
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

template<class T>
const AVLNode<T> *AVLTree<T>::GetMinNode() const {
    if (root == nullptr)
        return nullptr;
    return root->GetMinNode();
}




#endif //MAIN_CPP_AVLTTREE_H


