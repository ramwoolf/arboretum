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
    std::cout << std::boolalpha;
    Arboretum::BinaryTree<int> my_tree{42, 6, 34, 18, 41, 8};
    my_tree.preorder_tree_walk();
    auto t_max = my_tree.maximum();
    auto t_min = my_tree.minimum();
    std::cout << t_max << ", " << t_min << std::endl;
    std::cout << my_tree.tree_search(41) << std::endl;
    std::cout << my_tree.tree_search(481) << std::endl;
    
    try
    {
        auto s6 = my_tree.successor(6);
        std::cout << "Successor for 6 is " << s6 << std::endl;
        auto s7 = my_tree.successor(7);
        std::cout << "Successor for 7 is " << s7 << std::endl;
    }
    catch(const std::logic_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // auto s = my_tree.successor(6);
    // auto p = my_tree.predecessor();
    // std::cout << s << ", " << p << std::endl;
    // my_tree.remove_from_tree(18);
    // my_tree.remove_from_tree(10);
    return 0;
}
