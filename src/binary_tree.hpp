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
        {}

        BinaryTreeNode(T const &key_, std::unique_ptr<BinaryTreeNode> left_, std::unique_ptr<BinaryTreeNode> right_)
            : key(key_), left(std::move(left_)), right(std::move(right_)) 
        {}

        BinaryTreeNode(T &&key_, std::unique_ptr<BinaryTreeNode> left_, std::unique_ptr<BinaryTreeNode> right_)
            : key(key_), left(std::move(left_)), right(std::move(right_))
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

        BinaryTree(BinaryTree&& arg) : root(arg.root){}
        BinaryTree<T>& operator= (BinaryTree<T>&& arg)
        {
            root = std::move(arg.root);
            return *this;
        }

        bool is_empty() const
        {
            return root == nullptr;
        }

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
            }
            else
            {
                y->right = std::make_unique<BinaryTreeNode<T>>(std::move(new_key));
            }
        }

        std::unique_ptr<T> minimum() const
        {
            if (is_empty())
            {
                return nullptr;
            }
            return tree_minimum(root);
        }

        std::unique_ptr<T> maximum() const
        {
            if (is_empty())
            {
                return nullptr;
            }
            return tree_maximum(root);
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

        // void remove_from_tree(T&& key)
        // {
        //     auto found_key = search(std::move(key));
        //     if (found_key == nullptr)
        //     {
        //         std::cout << key << " not found" << std::endl;
        //         return;
        //     }
            
        //     if (found_key->left == nullptr)
        //     {
        //         transplant(found_key, found_key->right.get());
        //     }
        //     else if (found_key->right == nullptr)
        //     {
        //         transplant(found_key, found_key->left.get());
        //     }
        //     else
        //     {
        //         auto right_subminimum = tree_minimum(found_key->right.get());
        //         if (right_subminimum != found_key)
        //         {
        //             transplant(right_subminimum, right_subminimum->right.get());
        //             right_subminimum->right.reset(found_key->right.get());
        //             right_subminimum->right->parent = right_subminimum;
        //         }
        //         transplant(found_key, right_subminimum);
        //         right_subminimum->left.reset(found_key->left.get());
        //         right_subminimum->left->parent = right_subminimum;
        //     }
        //     return;
        // }

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

        std::unique_ptr<T> tree_minimum(TreeNodePtr<T> const &subroot) const
        {
            if (subroot->left == nullptr)
            {
                return std::make_unique<T>(subroot->key);
            }
            return tree_minimum(subroot->left);
        }

        std::unique_ptr<T> tree_maximum(TreeNodePtr<T> const &subroot) const
        {
            if (subroot->right == nullptr)
            {
                return std::make_unique<T>(subroot->key);
            }
            return tree_minimum(subroot->right);
        }

        // void transplant(BinaryTreeNode<T>* u, BinaryTreeNode<T>* v)
        // {
        //     if (u->parent == nullptr)
        //     {
        //         root.reset(v);
        //     }
        //     else if (u == u->parent->left.get())
        //     {
        //         u->parent->left.reset(v);
        //     }
        //     else
        //     {
        //         u->parent->right.reset(v);
        //     }
        //     if (v != nullptr)
        //     {
        //         v->parent = u->parent;
        //     }
        // }
        
    };
}

#endif
