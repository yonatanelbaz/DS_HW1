#ifndef MAIN
#define MAIN

#include <iostream>
#include "AVLTtree.h"



 int greater1(const int& value, const int& node_value){
    if (value > node_value){

        return 1;
    }
    else if (value < node_value){
        return 2;
    }
    return 0;
}


int main() {
    AVLTree<int> tree = AVLTree<int>(greater1);
    /*for(int i = 1; i < 7; i++){
        if(i != 0)
            tree.Insert(i);
            std::cout << "tree"<<  std::endl;
            tree.print2DUtil((tree.get_root()), 5);
            std::cout <<  std::endl;
            std::cout <<  std::endl;
            std::cout <<  std::endl;
            std::cout <<  std::endl;
            std::cout <<  std::endl;
            std::cout <<  std::endl;
            std::cout << "heights"<<  std::endl;
            tree.print2DUtil2((tree.get_root()), 5);
    }*/
    tree.Insert(0);
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(3);
    tree.Insert(4);
    tree.Insert(5);
    tree.Insert(6);

    std::cout << "tree"<<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout << "heights"<<  std::endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#endif