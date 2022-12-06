#ifndef MAIN
#define MAIN

#include <iostream>
#include "AVL_tree.h"



 int greater1(int value, int node_value){
    if (value > node_value){

        return 1;
    }
    else if (value < node_value){
        return 2;
    }
    return 0;
}


int main() {
    int x = 0;
    AVL_tree<int> tree = AVL_tree<int>(x, &greater1);
    /*for(int i = 1; i < 7; i++){
        if(i != 0)
            tree.insert(i);
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

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);

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
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#endif