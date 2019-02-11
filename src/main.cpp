#ifndef MAJOR_VERSION_NUMBER
#define MAJOR_VERSION_NUMBER 0
#endif

#ifndef MINOR_VERSION_NUMBER
#define MINOR_VERSION_NUMBER 0
#endif


#include <iostream>

#include "binary_tree.hpp"

int main(int argc, char const *argv[])
{
    TreesLib::BinaryTree<int> my_tree{4, 6, 34, 18, 41, 8};
    my_tree.preorder_tree_walk();
    return 0;
}
