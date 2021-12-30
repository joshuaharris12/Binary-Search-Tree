#ifndef TREE_H
#define TREE_H

#include "treenode.h"

template <typename T>
class BinarySearchTree {


private:

    unique_ptr<TreeNode<T>> root;

public:

    BinarySearchTree() 
        : root(nullptr) {}

    BinarySearchTree(const BinarySearchTree<T> & other) {
        createNewTree(other.getRoot().get(), nullptr);
    }

    BinarySearchTree & operator=(const BinarySearchTree<T> & other) {
        root = unique_ptr<TreeNode<T>>(other.root.get());
        return *this;
    }

    void write(ostream & o) const {
        root.get()->write(o);
    }

     TreeNode<T> * insert(T data) {
        TreeNode<T> * current = root.get(); 
        if (current == nullptr) {
            TreeNode<T> * newNode = new TreeNode<T>(data);
            root.release();
            root = unique_ptr<TreeNode<T>>(newNode);
            return newNode;
        } else {
            bool found = false;
            while(!found) {
                if (data < current->data) {
                    if (current->leftChild != nullptr) {
                        current = current->leftChild.get();
                    } else {
                        found = true;
                    }
                } else if (current->data < data) {
                    if (current->rightChild != nullptr) {
                        current = current->rightChild.get();
                    } else {
                        found = true;
                    }
                } else {
                    return current;
                }
            }
            TreeNode<T> * newNode = new TreeNode<T>(data);
            newNode->parent = current;
            if (data < current->data) {
                current->setLeftChild(newNode);
            } else {
                current->setRightChild(newNode);
            }

            if (maxDepth() > 2) {
                
                if (newNode->parent == root.get()) {
                    return newNode;
                } else {
                    TreeNode<T> * PPN = newNode->parent->parent;
                    TreeNode<T> * PN = newNode->parent;
                    int leftValue;
                    int rightValue;
                    if (PPN->leftChild == nullptr) {
                        leftValue = 0;
                    } else {
                        leftValue = PPN->leftChild.get()->maxDepth();
                    }
                    if (PPN->rightChild == nullptr) {
                        rightValue = 0;
                    } else {
                        rightValue = PPN->rightChild.get()->maxDepth();
                    }
                    int balFactor = leftValue - rightValue; // balence factor of tree
                    if (balFactor == 2 || balFactor == -2) {
                        if (PPN->rightChild != nullptr && PPN->rightChild.get()->rightChild != nullptr) {
                            leftRotation(PPN, newNode, PN); // left rotation
                        } else if (PPN->leftChild != nullptr && PPN->leftChild.get()->leftChild != nullptr) {
                            rightRotation(PPN, newNode, PN); // right rotation
                        } else if (PPN->leftChild != nullptr && PPN->leftChild.get()->rightChild != nullptr) {
                            leftRightRotation(PPN, newNode, PN); // left right rotation
                        } else if (PPN->rightChild != nullptr && PPN->rightChild.get()->leftChild != nullptr) {
                            rightLeftRotation(PPN, newNode, PN); // right left rotation
                        }
                    }
                }
            }
            return newNode; 
        }
    }

    TreeNode<T> * find(const T data) const {
        if (root == nullptr) {
            return nullptr;
        } else {
            TreeNode<T> * current = root.get(); // start with the root
            bool not_found = false;
            while(!not_found) {
                if (data < current->data) {
                    if (current->leftChild != nullptr) {
                        current = current->leftChild.get();
                    } else {
                        not_found = true;
                    }
                } else if (current->data < data) {
                    if (current->rightChild != nullptr) {
                        current = current->rightChild.get();
                    } else {
                        not_found = true;
                    }
                } else {
                    return current;
                }
            }
            return nullptr;
        }
    }


    TreeNodeIterator<T> begin() {
        TreeNode<T> * current = root.get();
        if (current == nullptr) {
            return nullptr;
        } else if (current->leftChild == nullptr && current->rightChild == nullptr) {
            return TreeNodeIterator<T>(current);
        } else if (current->leftChild == nullptr && current->rightChild != nullptr) {
            return TreeNodeIterator<T>(current);
        } else {
            while(current->leftChild != nullptr) {
                current = current->leftChild.get();
            }
            return TreeNodeIterator<T>(current);
        }
    }

    TreeNodeIterator<T> end() {
        return nullptr;
    }


    int maxDepth() const {
       return root.get()->maxDepth();
    }

    const unique_ptr<TreeNode<T>> & getRoot() const {
        return root;
    }

    


private:

    void leftRotation(TreeNode<T> * PPN, TreeNode<T> * newNode, TreeNode<T> * PN) {
        if (PPN->rightChild.get()->rightChild.get() == newNode) {
            if (PPN == root.get()) {
                TreeNode<T> * rootReleased = root.release();
                root = unique_ptr<TreeNode<T>>(PN);
                rootReleased->rightChild.release();
                root.get()->setLeftChild(rootReleased);
                rootReleased->parent = root.get();
                root.get()->parent = nullptr;
            } else {
                TreeNode<T> * rootPPN = PPN->parent;
                if (rootPPN->leftChild != nullptr && rootPPN->leftChild.get() == PPN) {
                    if (rootPPN->leftChild.get() == PPN) {
                        TreeNode<T> * nodeReleased = rootPPN->leftChild.release();
                        //rootPPN->leftChild = unique_ptr<TreeNode<T>>(PN);
                        rootPPN->setLeftChild(PN);
                        nodeReleased->parent = PN;
                        nodeReleased->rightChild.release();
                        PN->setLeftChild(nodeReleased);
                        PN->parent = rootPPN;
                    }
                } else if (rootPPN->rightChild != nullptr && rootPPN->rightChild.get() == PPN) {
                    if (rootPPN->rightChild.get() == PPN) {
                        TreeNode<T> * nodeReleased = rootPPN->rightChild.release();
                        //rootPPN->rightChild = unique_ptr<TreeNode<T>>(PN);
                        rootPPN->setRightChild(PN);
                        nodeReleased->parent = PN;
                        nodeReleased->rightChild.release();
                        PN->setLeftChild(nodeReleased);
                        PN->parent = rootPPN;
                    }
                }   
            }
        }
    }

    void rightRotation(TreeNode<T> * PPN, TreeNode<T> * newNode, TreeNode<T> * PN) {
        if (PPN->leftChild.get()->leftChild.get() == newNode) {
            if (PPN == root.get()) {
                TreeNode<T> * rootReleased = root.release();
                root = unique_ptr<TreeNode<T>>(PN);
                rootReleased->leftChild.release();
                root.get()->setRightChild(rootReleased);
                rootReleased->parent = root.get();
                root.get()->parent = nullptr;
            } else {
                TreeNode<T> * rootPPN = PPN->parent;
                if (rootPPN->leftChild != nullptr && rootPPN->leftChild.get() == PPN) {
                    if (rootPPN->leftChild.get() == PPN) {
                        TreeNode<T> * nodeReleased = rootPPN->leftChild.release();
                        rootPPN->setLeftChild(PN);
                        nodeReleased->parent = PN;
                        nodeReleased->leftChild.release();
                        PN->setRightChild(nodeReleased);
                        PN->parent = rootPPN;
                    }
                } else if (rootPPN->rightChild != nullptr && rootPPN->rightChild.get() == PPN) {
                    if (rootPPN->rightChild.get() == PPN) {
                        TreeNode<T> * nodeReleased = rootPPN->rightChild.release();
                        rootPPN->setRightChild(PN);
                        nodeReleased->parent = PN;
                        nodeReleased->leftChild.release();
                        PN->setRightChild(nodeReleased);
                        PN->parent = rootPPN;
                    }
                }   
            }
        } 
    }

    void leftRightRotation(TreeNode<T> * PPN, TreeNode<T> * newNode, TreeNode<T> * PN) {
        if (PPN->leftChild.get()->rightChild.get() == newNode) {
            TreeNode<T> * A = PPN->leftChild.release();
            PPN->setLeftChild(newNode);
            newNode->parent = PPN;
            PN->parent = newNode;
            newNode->setLeftChild(A);
            PN->rightChild.release();
            rightRotation(PPN, PN, newNode); // swap around newNode and PN as the nodes have switched positions
        }
    }

    void rightLeftRotation(TreeNode<T> * PPN, TreeNode<T> * newNode, TreeNode<T> * PN) {
        if (PPN->rightChild.get()->leftChild.get() == newNode) {
            TreeNode<T> * A = PPN->rightChild.release();
            PPN->setRightChild(newNode);
            newNode->parent = PPN;
            PN->parent = newNode;
            newNode->setRightChild(A);
            PN->leftChild.release();
            leftRotation(PPN, PN, newNode); // swap around newNode and PN as the nodes have switched positions
        }
    }

    TreeNode<T> * createNewTree(const TreeNode<T> * otn, TreeNode<T> * parentNode) {
        if (otn == nullptr) {
            return nullptr;
        } else {
            TreeNode<T> * newNode = new TreeNode<T>(otn->data);
            newNode->parent = parentNode;
            if(otn->parent == nullptr) {
                root.release();
                root = unique_ptr<TreeNode<T>>(newNode);
            }
            if (otn->leftChild != nullptr) {
                newNode->setLeftChild(createNewTree(otn->leftChild.get(), newNode));
            }
            if (otn->rightChild != nullptr) {
                newNode->setRightChild(createNewTree(otn->rightChild.get(), newNode));
            }
            return newNode;
        }
    }
};



#endif
