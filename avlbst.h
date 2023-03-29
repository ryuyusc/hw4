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
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int diff);
    void rotateRight(AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);


};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n) {
  //case where the node is the root
  if (this->root_ == n) {
    AVLNode<Key, Value>* l = n->getLeft();
    l->setParent(NULL);
    n->setParent(l);
    this->root_ = l;

    if (l->getRight() != NULL) {
      n->setLeft(l->getRight());
      l->getRight()->setParent(n);
    }
    else {
      n->setLeft(NULL);
    }
    
    l->setRight(n);
  }
  else {
    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* l = n->getLeft();
    n->setParent(l);
    l->setParent(p);

    if (n == p->getRight()) {
      p->setRight(l);
    }
    else {
      p->setLeft(l);
    }

    if (l->getRight() != NULL) {
      n->setLeft(l->getRight());
      l->getRight()->setParent(n);
    }
    else {
      n->setLeft(NULL);
    }

    l->setRight(n);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n) {
  //case where the node is the root
  if (this->root_ == n) {
    AVLNode<Key, Value>* r = n->getRight();
    r->setParent(NULL);
    n->setParent(r);
    this->root_ = r;

    if (r->getLeft() != NULL) {
      n->setRight(r->getLeft());
      r->getLeft()->setParent(n);
    }
    else {
      n->setRight(NULL);
    }
    
    r->setLeft(n);
  }
  else {
    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* r = n->getRight();
    n->setParent(r);
    r->setParent(p);

    if (n == p->getRight()) {
      p->setRight(r);
    }
    else {
      p->setLeft(r);
    }

    if (r->getLeft() != NULL) {
      n->setRight(r->getLeft());
      r->getLeft()->setParent(n);
    }
    else {
      n->setRight(NULL);
    }

    r->setLeft(n);
  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* temp = ((AVLNode<Key, Value>*)(this->root_));

    // TODO
    if (temp == NULL) {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      ((AVLNode<Key, Value>*)(this->root_))->setBalance(0);
      return;
    }
    else {
      //creates temporary Node pointer to traverse through BST
      while (temp != NULL) {
        //case for same key find
        if (temp->getKey() == new_item.first) {
          temp->setValue(new_item.second);
          return;
        }
        //go to left child
        if (new_item.first < temp->getKey()) {
          //if left child does not exist
          if (temp->getLeft() == NULL) {
            AVLNode<Key, Value>* toAdd = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
            temp->setLeft(toAdd);
            temp = temp->getLeft();
            break;
          }
          else {
            temp = temp->getLeft();
          }
        }
        //go to right child
        else if (new_item.first >= temp->getKey()) {
          //if right child does not exist
          if (temp->getRight() == NULL) {
            AVLNode<Key, Value>* toAdd = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
            temp->setRight(toAdd);
            temp = temp->getRight();
            break;
          }
          else {
            temp = temp->getRight();
          }
        }
      }
    }

    //end of inserting part

    temp->setBalance(0);

    //if the node that was added is not the root
    if (temp->getParent() != NULL) {
        if (temp->getParent()->getBalance() == -1) {
            temp->getParent()->setBalance(0);
        }
        else if (temp->getParent()->getBalance() == 1) {
            temp->getParent()->setBalance(0);
        }
        else if (temp->getParent()->getBalance() == 0) {
            if (temp == temp->getParent()->getLeft()) {
                temp->getParent()->setBalance(-1);
            }
            else {
                temp->getParent()->setBalance(1);
            }
            insertFix(temp->getParent(), temp);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
    if (p == NULL || p->getParent() == NULL) {
        return;
    }

    AVLNode<Key, Value>* g = p->getParent();

    int8_t leftOrRight = -1;

    if (p == g->getRight()) {
      leftOrRight = 1;
    }

    g->setBalance(g->getBalance() + leftOrRight);
      
    if (g->getBalance() == 0) {
      return;
    }
    else if (g->getBalance() == leftOrRight) {
      insertFix(g, p);
    }
    else {
      //zig zig left
      if (n == p->getLeft() && p == g->getLeft()) {
        rotateRight(g);
        g->setBalance(0);
        p->setBalance(0);
      }

      //zig zig right
      else if (n == p->getRight() && p == g->getRight()) {
        rotateLeft(g); 
        g->setBalance(0);
        p->setBalance(0);
      }

      //zig zag left right
      else if (n == p->getRight() && p == g->getLeft()) {
        rotateLeft(p);
        rotateRight(g);
        switch (n->getBalance()) {
          case -1:
            n->setBalance(0);
            p->setBalance(0);
            g->setBalance(1);
            break;
          case 0:
            p->setBalance(0);
            g->setBalance(0);
            break;
          case 1:
            n->setBalance(0);
            p->setBalance(-1);
            g->setBalance(0);
            break;
        }
      }

      //zig zag right left
      else if (n == p->getLeft() && p == g->getRight()) {
        rotateRight(p);
        rotateLeft(g);
        switch (n->getBalance()) {
          case -1:
            n->setBalance(0);
            p->setBalance(1);
            g->setBalance(0);
            break;
          case 0:
            p->setBalance(0);
            g->setBalance(0);
            break;
          case 1:
            n->setBalance(0);
            p->setBalance(0);
            g->setBalance(-1);
            break;
        }
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
    // TODO
    AVLNode<Key, Value>* temp = (AVLNode<Key, Value>*)this->internalFind(key);
    int diff = 0;

    //case where key is found
    if (temp != NULL) {
      //case for 2 children 
      if (temp->getRight() != NULL && temp->getLeft() != NULL) {
        nodeSwap((AVLNode<Key, Value>*)(this->predecessor(temp)), temp);

        if (temp->getParent() != NULL) {
          if (temp == temp->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }

        //case for 1 child
        if (temp->getRight() != NULL) {
          //if temp is a left child
          if (temp == temp->getParent()->getLeft()) {
            temp->getParent()->setLeft(temp->getRight());
            temp->getRight()->setParent(temp->getParent());
          }
          //else temp is a right child
          else {
            temp->getParent()->setRight(temp->getRight());
            temp->getRight()->setParent(temp->getParent());
          }
        }
        else if (temp->getLeft() != NULL) {
          if (temp == temp->getParent()->getLeft()) {
            temp->getParent()->setLeft(temp->getLeft());
            temp->getLeft()->setParent(temp->getParent());
          }
          else {
            temp->getParent()->setRight(temp->getLeft());
            temp->getLeft()->setParent(temp->getParent());
          }
        }
        //case for 0 children
        else {
          if (temp == temp->getParent()->getLeft()) {
            temp->getParent()->setLeft(NULL);
          }
          else {
            temp->getParent()->setRight(NULL);
          }
        }
      }

      //case for 1 child
      else if (temp->getRight() != NULL) {
        //if temp is the root
        if (temp->getParent() != NULL) {
          if (temp == temp->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }
        if (temp == this->root_) {
          this->root_ = temp->getRight();
          temp->getRight()->setParent(NULL);
        }
        //if temp is a left child
        else if (temp == temp->getParent()->getLeft()) {
          temp->getParent()->setLeft(temp->getRight());
          temp->getRight()->setParent(temp->getParent());
        }
        //else temp is a right child
        else {
          temp->getParent()->setRight(temp->getRight());
          temp->getRight()->setParent(temp->getParent());
        }
      }
      else if (temp->getLeft() != NULL) {
        if (temp->getParent() != NULL) {
          if (temp == temp->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }
        if (temp == this->root_) {
          this->root_ = temp->getLeft();
          temp->getLeft()->setParent(NULL);
        }
        else if (temp == temp->getParent()->getLeft()) {
          temp->getParent()->setLeft(temp->getLeft());
          temp->getLeft()->setParent(temp->getParent());
        }
        else {
          temp->getParent()->setRight(temp->getLeft());
          temp->getLeft()->setParent(temp->getParent());
        }
      }


      //case for 0 children
      else {
        if (temp->getParent() != NULL) {
          if (temp == temp->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }
        if (temp == this->root_) {
          this->root_ = NULL;
        }
        else if (temp == temp->getParent()->getLeft()) {
          temp->getParent()->setLeft(NULL);
        }
        else {
          temp->getParent()->setRight(NULL);
        }
      }
      AVLNode<Key, Value>* p = temp->getParent();

      delete temp;

      removeFix(p, diff);
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff) {
  if (n == NULL) {
    return;
  }


  AVLNode<Key, Value>* p = n->getParent();
  int nextDiff = 0;

  if (n->getParent() != NULL) {
    if (n == p->getLeft()){
      nextDiff = 1;
    }
    else {
      nextDiff = -1;
    }
  }


  //right node
  if (n->getBalance() + diff == -2) {
    int leftHeight = this->depth(n->getLeft());
    int rightHeight = this->depth(n->getRight());
    AVLNode<Key, Value>* c = n->getLeft();
    if (rightHeight > leftHeight) {
      c = n->getRight();
    }
    if (c->getBalance() == -1) {
      rotateRight(n);
      n->setBalance(0);
      c->setBalance(0);
      removeFix(p, nextDiff);
    }
    else if (c->getBalance() == 0) {
      rotateRight(n);
      n->setBalance(-1);
      c->setBalance(1);
    }
    else if (c->getBalance() == 1) {
      AVLNode<Key, Value>* g = c->getRight();
      rotateLeft(c);
      rotateRight(n);
      if (g->getBalance() == 1) {
        n->setBalance(0);
        c->setBalance(-1);
        g->setBalance(0);
      }
      else if (g->getBalance() == 0) {
        n->setBalance(0);
        c->setBalance(0);
        g->setBalance(0);
      }
      else if (g->getBalance() == -1) {
        n->setBalance(1);
        c->setBalance(0);
        g->setBalance(0);
      }
      removeFix(p, nextDiff);
    }
  }

  //left node
  else if (n->getBalance() + diff == 2) {
    int leftHeight = this->depth(n->getLeft());
    int rightHeight = this->depth(n->getRight());
    AVLNode<Key, Value>* c = n->getLeft();
    if (rightHeight > leftHeight) {
      c = n->getRight();
    }

    if (c->getBalance() == 1) {
      rotateLeft(n);
      n->setBalance(0);
      c->setBalance(0);
      removeFix(p, nextDiff);
    }
    else if (c->getBalance() == 0) {
      rotateLeft(n);
      n->setBalance(1);
      c->setBalance(-1);
    }
    else if (c->getBalance() == -1) {
      AVLNode<Key, Value>* g = c->getLeft();
      rotateRight(c);
      rotateLeft(n);
      if (g->getBalance() == -1) {
        n->setBalance(0);
        c->setBalance(1);
        g->setBalance(0);
      }
      else if (g->getBalance() == 0) {
        n->setBalance(0);
        c->setBalance(0);
        g->setBalance(0);
      }
      else if (g->getBalance() == 1) {
        n->setBalance(-1);
        c->setBalance(0);
        g->setBalance(0);
      }
      removeFix(p, nextDiff);
    }
  }

  else if (n->getBalance() + diff == -1) {
    n->setBalance(-1);
  }
  else if (n->getBalance() + diff == 1) {
    n->setBalance(1);
  }

  else if (n->getBalance() + diff == 0) {
    n->setBalance(0);
    removeFix(p, nextDiff);
  }
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
