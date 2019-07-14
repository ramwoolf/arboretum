#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <iostream>
#include <memory>
#include <stack>
#include <cassert>
#include <initializer_list>
#include <exception>

namespace Arboretum
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

        BinaryTreeNode<T>* parent;

        BinaryTreeNode(T&& k) 
        : key{std::move(k)}
        , left{nullptr}
        , right{nullptr}
        , parent{nullptr}
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
        BinaryTree() : root{nullptr} {}
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
                        root->parent = nullptr;
                    }
                    else if (tmp < y->key)
                    {
                        y->left = std::make_unique<BinaryTreeNode<T>>(std::move(tmp));
                        y->left->parent = y;
                    }
                    else
                    {
                        y->right = std::make_unique<BinaryTreeNode<T>>(std::move(tmp));
                        y->right->parent = y;
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
                root->parent = nullptr;
            }
            else if (new_key < y->key)
            {
                y->left = std::make_unique<BinaryTreeNode<T>>(std::move(new_key));
                y->left->parent = y;
            }
            else
            {
                y->right = std::make_unique<BinaryTreeNode<T>>(std::move(new_key));
                y->right->parent = y;
            }
        }

        T minimum() const
        {
            return tree_minimum(root.get());
        }

        T maximum() const
        {
            return tree_maximum(root.get());
        }

        T successor(T&& key) const
        {
            auto subroot = search(std::move(key));
            if (subroot)
                return tree_successor(subroot);
            else
                throw std::logic_error("Key error");
            
        }

        T predecessor(T&& key) const
        {
            auto subroot = search(std::move(key));
            if (subroot)
                return tree_predecessor(subroot);
            else
                throw std::logic_error("Key error");
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

        bool is_key_exists(T&& seek_key)
        {
            return search(std::move(seek_key)) != nullptr;
        }

        void remove_from_tree(T&& key)
        {
            auto found_key = search(std::move(key));
            if (found_key == nullptr)
            {
                std::cout << key << " not found" << std::endl;
                return;
            }
            
            if (found_key->left == nullptr)
            {
                transplant(found_key, found_key->right.get());
            }
            else if (found_key->right == nullptr)
            {
                transplant(found_key, found_key->left(get()));
            }
            else
            {
                auto right_subminimum = tree_minimum(found_key->right.get());
                if (right_subminimum != found_key)
                {
                    transplant(right_subminimum, right_subminimum->right.get());
                    right_subminimum->right.get() = found_key->right.get();
                    right_subminimum->right->parent = right_subminimum;
                }
                transplant(found_key, right_subminimum);
                right_subminimum->left.get() = found_key->left.get();
                right_subminimum->left->parent = right_subminimum;
            }
            return;
        }

    private:
        BinaryTreeNode<T>* search(T&& seek_key) const
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
            return result;
        }

        T tree_minimum(BinaryTreeNode<T>* subroot) const
        {
            while (subroot->left != nullptr)
            {
                subroot = subroot->left.get();
            }
            return subroot->key;
        }

        T tree_maximum(BinaryTreeNode<T>* subroot) const
        {
            while (subroot->right != nullptr)
            {
                subroot = subroot->right.get();
            }
            return subroot->key;
        }

        T tree_successor(BinaryTreeNode<T>* subroot) const
        {
            if (tree_maximum(root.get()) == subroot->key)
            {
                return subroot->key;
            }
            if (subroot->right != nullptr)
            {
                return tree_minimum(subroot->right.get());
            }
            auto it = subroot->parent;

            while (it != nullptr && subroot == it->right.get())
            {
                std::cout << it->key << std::endl;
                subroot = it;
                it = it->parent;
            }
            return it->key;
        }

        T tree_predecessor(BinaryTreeNode<T>* subroot) const
        {
            if (tree_minimum(root.get()) == subroot->key)
            {
                return subroot->key;
            }
            if (subroot->left != nullptr)
            {
                return tree_maximum(subroot->left.get());
            }
            auto it = subroot->parent;
            while (it != nullptr && subroot == it->left.get())
            {
                subroot = it;
                it = it->parent;
            }
            return it->key;
        }

        void transplant(BinaryTreeNode<T>* u, BinaryTreeNode<T>* v)
        {
            if (u->parent == nullptr)
            {
                root.get() = v;
            }
            else if (u == u->parent->left.get())
            {
                u->parent->left.get() = v;
            }
            else
            {
                u->parent->right.get() = v;
            }
            if (v != nullptr)
            {
                v->parent = u->parent;
            }
        }
        
    };
}

#endif
