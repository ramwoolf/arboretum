#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <iostream>
#include <memory>
#include <stack>
#include <cassert>
#include <initializer_list>
#include <exception>

namespace TreesLib
{
    template <typename T> struct BinaryTreeNode;

    template <typename T>
    using TreeNodePtr = std::unique_ptr<BinaryTreeNode<T>>;

    template <typename T>
    struct BinaryTreeNode
    {
        T key;
        TreeNodePtr<T> left;
        TreeNodePtr<T> right;

        BinaryTreeNode(T&& k) 
        : key(std::move(k))
        , left(nullptr)
        , right(nullptr)
        {}

        ~BinaryTreeNode()
        {
            std::cout << "Dtor " << key << std::endl;
        }
    };

    template <typename T>
    class BinaryTree
    {
    private:
        TreeNodePtr<T> root;

    public:
        BinaryTree() : root(nullptr) {}
        BinaryTree(std::initializer_list<T> l) : root(nullptr)
        {
            try
            {
                for (auto it = l.begin(); it != l.end(); ++it)
                {
                    auto tmp = *it;
                    BinaryTreeNode<T>* y = nullptr;
                    BinaryTreeNode<T>* x = root.get();

                    while (x != nullptr)
                    {
                        y = x;
                        x = (tmp < x->key) ? x->left.get() : x->right.get();
                    }

                    if (y == nullptr)
                    {
                        root = std::make_unique<BinaryTreeNode<T>>(std::move(tmp));
                    }
                    else if (tmp < y->key)
                    {
                        y->left = std::make_unique<BinaryTreeNode<T>>(std::move(tmp));
                    }
                    else
                    {
                        y->right = std::make_unique<BinaryTreeNode<T>>(std::move(tmp));
                    }
                }
            }
            catch (const std::bad_alloc& e)
            {
                std::cerr << e.what() << '\n';
                root = nullptr;
                throw;
            }
        }

        BinaryTree(const BinaryTree<T>& arg) = delete;
        BinaryTree<T>& operator= (const BinaryTree<T>& arg) = delete;

        void insert(T&& new_key)
        {
            BinaryTreeNode<T>* y = nullptr;
            BinaryTreeNode<T>* x = root.get();

            while (x != nullptr)
            {
                y = x;
                x = (new_key < x->key) ? x->left.get() : x->right.get();
            }

            if (y == nullptr)
            {
                root = std::make_unique<BinaryTreeNode<T>>(std::move(new_key));
            }
            else if (new_key < y->key)
            {
                y->left = std::make_unique<BinaryTreeNode<T>>(std::move(new_key));
                // y->left->parent = y;
            }
            else
            {
                y->right = std::make_unique<BinaryTreeNode<T>>(std::move(new_key));
                // y->right->parent = y;
            }
        }

        void preorder_tree_walk() const
        {
            std::stack<BinaryTreeNode<T>*> s;
            BinaryTreeNode<T>* it = root.get();

            std::cout << "Preorder walk" << std::endl;

            while (it != nullptr || s.size() > 0)
            {
                if (it != nullptr)
                {
                    std::cout << it->key << " ";
                    s.push(it);
                    it = it->left.get();
                }
                else
                {
                    it = s.top()->right.get();
                    s.pop();
                    if (it != nullptr)
                    {
                        std::cout << it->key << " ";
                        s.push(it);
                        it = it->left.get();
                    }
                }
            }
            std::cout << std::endl;
        }

        T tree_search(T&& seek_key)
        {
            BinaryTreeNode<T>* result = root.get();

            while (result != nullptr && result->key != seek_key)
            {
                if (seek_key < result->key)
                {
                    result = result->left.get();
                }
                else
                {
                    result = result->right.get();
                }
            }
            return result->key;
        }
    };
}

#endif
