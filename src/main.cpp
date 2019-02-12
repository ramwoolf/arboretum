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
    TreesLib::BinaryTree<int> my_tree{42, 6, 34, 18, 41, 8};
    my_tree.preorder_tree_walk();
    auto t_max = my_tree.maximum();
    auto t_min = my_tree.minimum();
    std::cout << t_max << ", " << t_min << std::endl;
    auto s = my_tree.successor();
    auto p = my_tree.predecessor();
    std::cout << s << ", " << p << std::endl;
    // my_tree.remove_from_tree(18);
    // my_tree.remove_from_tree(10);
    return 0;
}
