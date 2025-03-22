#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void rotateRight(AVLNode<Key, Value>* n);
    virtual void rotateLeft(AVLNode<Key, Value>* n);
    virtual void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    virtual void removeFix(AVLNode<Key, Value>* n, int8_t diff);


};
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n){
    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* lc = n->getLeft();
    if(lc == NULL)
      return;
    AVLNode<Key, Value>* lrc = lc->getRight();
    //change new root
    lc->setParent(p);
    lc->setRight(n);
    
    if(p == NULL)
      this->root_ = lc;
    else{
      if(n == p->getLeft())
        p->setLeft(lc);
      else
        p->setRight(lc);
    }
    //set new n
    n->setParent(lc);
    n->setLeft(lrc);
    if(lrc != NULL)
      lrc->setParent(n);
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n){
    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* rc = n->getRight();
    if(rc == NULL)
      return;
    AVLNode<Key, Value>* rlc = rc->getLeft();
    //change new root
    rc->setParent(p);
    rc->setLeft(n);
    n->setParent(rc);
    if(p == NULL)
      this->root_ = rc;
    else{
      if(n == p->getLeft())
        p->setLeft(rc);
      else
        p->setRight(rc);
    }
    //set new n
    n->setRight(rlc);
    if(rlc != NULL)
      rlc->setParent(n);
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){
    if(p == NULL || p->getParent() == NULL)
      return;
    AVLNode<Key, Value>* g = p->getParent();
    if(p == g->getLeft()){
      g->updateBalance(-1);
      int8_t b = g->getBalance();
      if(b == 0)
        return;
      else if (b == -1)
        insertFix(g, p);
      else if (b == -2){
        //zig-zig
        if(p->getBalance() == -1){
          rotateRight(g);
          g->setBalance(0), p->setBalance(0);
        }
        else if (p->getBalance() == 1){
          rotateLeft(p);
          rotateRight(g);
          if(n->getBalance() == -1)
            p->setBalance(0), g->setBalance(1), n->setBalance(0);
          else if (n->getBalance() == 0)
            p->setBalance(0), g->setBalance(0), n->setBalance(0);
          else if (n->getBalance() == 1)
            p->setBalance(-1), g->setBalance(0), n->setBalance(0);
        }
      }
    } else {
      g->updateBalance(1);
      int8_t b = g->getBalance();
      if(b == 0)
        return;
      else if (b == 1)
        insertFix(g, p);
      else if (b == 2){
        //zig-zig
        if(p->getBalance() == 1){
          rotateLeft(g);
          g->setBalance(0), p->setBalance(0);
        }
        else if (p->getBalance() == -1){
          rotateRight(p);
          rotateLeft(g);
          if(n->getBalance() == 1)
            p->setBalance(0), g->setBalance(-1), n->setBalance(0);
          else if (n->getBalance() == 0)
            p->setBalance(0), g->setBalance(0), n->setBalance(0);
          else if (n->getBalance() == -1)
            p->setBalance(1), g->setBalance(0), n->setBalance(0);
        }
      }
    }
}
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
  if(this->root_ == NULL)
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
  AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value>* p;
  while(temp != NULL){
    Key key = temp->getKey();
    if(key == new_item.first){
      temp->setValue(new_item.second);
      break;
    } else if (new_item.first < key){
            p = temp;
            temp = temp->getLeft();
            if(temp == NULL){
                p->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, p));
                if(p->getBalance() == -1 || p->getBalance() == 1)
                  p->setBalance(0);
                else
                  p->setBalance(-1), insertFix(p, p->getLeft());
            }

    } else{
        p = temp;
        temp = temp->getRight();
        if(temp == NULL){
            p->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, p));
            if(p->getBalance() == -1 || p->getBalance() == 1)
              p->setBalance(0);
            else
              p->setBalance(1), insertFix(p, p->getRight());
        }
    }
  }
}
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff){
  AVLNode<Key, Value>* p = n->getParent();
  int8_t ndiff;
  if(p != NULL){
    if(p->getLeft() == n)
      ndiff = 1;
    else
      ndiff = -1;
  }
  if(diff == -1){
    int8_t b = n->getBalance() + diff;
    if(b == -2){
      AVLNode<Key, Value>* c = n->getLeft();
      if(c -> getBalance()== -1){
        rotateRight(n);
        n->setBalance(0), c->setBalance(0);
        if(p != NULL)
          removeFix(p, ndiff);
      } else if(c->getBalance() == 0){
        rotateRight(n);
        n->setBalance(-1), c->setBalance(1);
      } else if(c->getBalance()== 1){
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c), rotateRight(n);
        n->setBalance(0), c->setBalance(0);
        if(g -> getBalance() == 1)
          n->setBalance(0), c->setBalance(-1), g->setBalance(0);
        else if(g -> getBalance() == 0)
          n->setBalance(0), c->setBalance(0), g->setBalance(0);
        else if(g -> getBalance() == -1)
          n->setBalance(1), c->setBalance(0), g->setBalance(0);
        if(p != NULL)
          removeFix(p, ndiff);
      }
    } else if (b == -1){
      n->setBalance(-1);
    } else if (b == 0) {
      n->setBalance(0);
      if (p != NULL)
        removeFix(p, ndiff);
    }
  } else if (diff == 1){
    int8_t b = n->getBalance() + diff;
    if(b == 2){
      AVLNode<Key, Value>* c = n->getRight();
      if(c -> getBalance()== 1){
        rotateLeft(n);
        n->setBalance(0), c->setBalance(0);
        if(p != NULL)
          removeFix(p, ndiff);
      } else if(c->getBalance() == 0){
        rotateLeft(n);
        n->setBalance(1), c->setBalance(-1);
      } else if(c->getBalance()== -1){
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c), rotateLeft(n);
        n->setBalance(0), c->setBalance(0);
        if(g -> getBalance() == -1)
          n->setBalance(0), c->setBalance(1), g->setBalance(0);
        else if(g -> getBalance() == 0)
          n->setBalance(0), c->setBalance(0), g->setBalance(0);
        else if(g -> getBalance() == 1)
          n->setBalance(-1), c->setBalance(0), g->setBalance(0);
        if(p != NULL)
          removeFix(p, ndiff);
      }
    } else if (b == 1){
      n->setBalance(1);
    } else if (b == 0) {
      n->setBalance(0);
      if (p != NULL)
        removeFix(p, ndiff);
    }
  }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    
    if(temp == NULL)
        return;
    if(temp->getLeft() != NULL && temp->getRight() != NULL){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(temp));
        if(pred != NULL)
            nodeSwap(temp, pred);
    }
    AVLNode<Key, Value>* p = temp->getParent();
    int8_t diff = 0;
    if(p != NULL){
      if (temp == p->getLeft())
        diff = 1;
      else
        diff = -1;
    }
    this->del(temp);
    if (p != NULL)
      removeFix(p, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
