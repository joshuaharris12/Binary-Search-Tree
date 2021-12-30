#ifndef TREEMAP_H
#define TREEMAP_H

#include "tree.h"

template<typename Key, typename Value>
class KeyValuePair {
    
public:
    
    const Key k;
    Value v;
    
    
    KeyValuePair(const Key & keyIn, const Value & valueIn)
        : k(keyIn), v(valueIn) {}
    
    KeyValuePair(const Key & keyIn) 
        : k(keyIn) {}  // set the value 

    bool operator<(const KeyValuePair & other) const {
        return (k < other.k);
    }
};


template<typename Key, typename Value>
ostream & operator<< (ostream & o, const KeyValuePair<Key,Value> & kv){
    o << kv.k << "," << kv.v;
    return o;
}


template<typename Key, typename Value>
class TreeMap {
  
private:
    BinarySearchTree<KeyValuePair<Key,Value> > tree;
    
public:
    
    KeyValuePair<Key,Value> * insert(const Key & k, const Value & v) {
        return &(tree.insert(KeyValuePair<Key,Value>(k,v))->data);
    }
    
    void write(ostream & o) const {
        tree.write(o);
    }
    
    KeyValuePair<Key, Value> * find(const Key & key) const {
        TreeNode<KeyValuePair<Key, Value> > * foundNode = tree.find(KeyValuePair<Key, Value>(key));
        if (foundNode != nullptr) {
            return &(foundNode->data);
        } 
        return nullptr;
    }
    
};


#endif
