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
    using TreeNodeRef = std::unique_ptr<BinaryTreeNode<T>>&;

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
                    insert(std::move(tmp), root);
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
            insert(std::move(new_key), root);
        }

        void remove(T const &key_to_remove)
        {
            remove(key_to_remove, root);
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

        bool is_key_exists(T const &seek_key)
        {
            return is_key_exists(seek_key, root);
        }        

    private:
        void insert(T&& new_key, TreeNodeRef<T> node)
        {
            if (node == nullptr)
            {
                node = std::make_unique<BinaryTreeNode<T>>(std::move(new_key), nullptr, nullptr);
            }
            else if (new_key < node->key)
            {
                insert(std::move(new_key), node->left);
            }
            else if (new_key > node->key)
            {
                insert(std::move(new_key), node->right);
            }
        }

        void remove(T const &key_to_remove, TreeNodeRef<T> node)
        {
            if (is_empty())
            {
                return;
            }
            if (key_to_remove < node->key)
            {
                remove(key_to_remove, node->left);
            }
            else if (key_to_remove > node->key)
            {
                remove(key_to_remove, node->right);
            }
            else if (node->left != nullptr && node->right != nullptr)
            {
                node->key = *tree_minimum(node->right);
                remove(node->key, node->right);
            }
            else
            {
                node = (node->left != nullptr) ? std::move(node->left) : std::move(node->right);
            }
        }

        bool is_key_exists(T const &seek_key, TreeNodeRef<T> node)
        {
            if (node == nullptr)
            {
                return false;
            }
            else if (seek_key < node->key)
            {
                return is_key_exists(seek_key, node->left);
            }
            else if (seek_key > node->key)
            {
                return is_key_exists(seek_key, node->right);
            }
            else
            {
                return true;
            }
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
    };
}

#endif
