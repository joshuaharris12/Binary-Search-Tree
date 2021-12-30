#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

#include <memory>
using std::unique_ptr;

#include <utility>
using std::pair;


template <typename T>
class TreeNode {

public:

    T data;
    unique_ptr<TreeNode<T>> leftChild;
    unique_ptr<TreeNode<T>> rightChild;
    TreeNode * parent;

    TreeNode(const T dataIn) 
        : data(dataIn), leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}

    void setLeftChild(TreeNode * child) {
        leftChild.release();
        leftChild = unique_ptr<TreeNode<T>>(child);
        leftChild.get()->parent = this;
    }

    void setRightChild(TreeNode * child) {
        rightChild.release();
        rightChild = unique_ptr<TreeNode<T>>(child);
        rightChild.get()->parent = this;
    }


    void write(ostream & o) const {
        if(leftChild) {
            leftChild.get()->write(o);
        }
        o << " ";
        o << data;
        o << " ";
        if (rightChild) {
            rightChild.get()->write(o);
        }
    }

    int maxDepth() {
            return (calculateTreeSize(this) + 1); // + 1 for to include the root node of the tree
        }

        int calculateTreeSize(const TreeNode<T> * currentNode) const {
            if (currentNode->leftChild == nullptr && currentNode->rightChild == nullptr) {
                return 0;
            } else {
                int leftTreeSize = 0;
                int rightTreeSize = 0; 
                if (currentNode->leftChild != nullptr) {
                    leftTreeSize = calculateTreeSize(currentNode->leftChild.get());
                }
                if (currentNode->rightChild != nullptr) {
                    rightTreeSize = calculateTreeSize(currentNode->rightChild.get());
                }
                if (leftTreeSize > rightTreeSize) {
                    return leftTreeSize + 1;
                } else {
                    return rightTreeSize + 1;
                }
            }
        }

};

template <typename T>
class TreeNodeIterator {

    private:

        TreeNode<T> * current;

        bool checkIfLastNode(TreeNode<T> * lastNode) const {
            TreeNode<T> * finding  = lastNode;
            if (finding == nullptr) {
                return true;
            } else if (finding->parent == nullptr && finding->rightChild != nullptr) {
                return false;
            } else {
                while(finding->parent != nullptr) {
                    if (finding == finding->parent->rightChild.get()) {
                        finding = finding->parent;
                    } else {
                    return false;
                    }
                }
                return true;
            }
        }

    public:

        TreeNodeIterator(TreeNode<T> * currentIn) 
            : current(currentIn) {}

        int & operator*() {
            return current->data;
        }

        bool operator==(const TreeNodeIterator<T> & other) const {
            return (current == other.current);
        }

        bool operator!=(const TreeNodeIterator<T> & other) const {
            return (current != other.current);
        }


        void operator++() {
            if (current != nullptr) {
                current = getNextTreeNode(current);
            }
        }

    private:

        TreeNode<T> * getParentNode(TreeNode<T> * currentNode) {
            while(currentNode->parent != nullptr && currentNode == currentNode->parent->rightChild.get()) {
                currentNode = currentNode->parent;
            }
            return currentNode->parent;
        }

     TreeNode<T> * getLeftMostTreeNode(TreeNode<T> * currentNode) {
            while(currentNode->leftChild.get() != nullptr) {
                currentNode = currentNode->leftChild.get();
            }
            return currentNode;
        }

        TreeNode<T> * getNextTreeNode(TreeNode<T> * currentNode) {
            if (currentNode->rightChild != nullptr) {
                return getLeftMostTreeNode(currentNode->rightChild.get());
            } else {
                 while(currentNode->parent != nullptr && currentNode == currentNode->parent->rightChild.get()) {
                     currentNode = currentNode->parent;
                 }
                return getParentNode(current);
            }
        }

};


#endif
