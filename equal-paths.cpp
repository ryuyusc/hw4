#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int depth(Node * root);

/**
 * @brief Returns true if all paths from leaves to root are the same length (height),
 *        and false otherwise
 *
 *        Note: this doesn't mean we are checking if the tree is full, but just that
 *        any leaf node (wherever it may exist) has the same length path to the root 
 *        as all others.
 * 
 * @param root Pointer to the root of the tree to check for equal paths
 */
bool equalPaths(Node * root)
{
    // Add your code below
    /*
    * will check for two leaves that are on
    * different levels and carry false all the way
    * up (trickles up kind of)
    */

    //checks if the root node is empty 
    if (root == NULL) {
      return true;
    }
    //checks if node is a leaf
    if (root->left == NULL && root->right == NULL) {
      return true;
    }

    //checks for leaves on either side
    bool l = equalPaths(root->left);
    bool r = equalPaths(root->right);

    //checks for leaf on one side and empty on other
    if (root->left == NULL || root->right == NULL) {
      return l && r;
    }

    //means there is a leaf on both the left and the right side
    if (l && r && (depth(root->left) != depth(root->right))) {
      //checks if the leaves are on different levels
      return false;
    }


    //will only return false if there is a single case
    //in which leaves are on different levels
    //then the false will trickle up
    return l && r;
}

int depth(Node * root) {
  if (root == NULL) {
    return 0;
  }

  int left = depth(root->left);
  int right = depth(root->right);

  if (left > right) {
    return left + 1;
  }
  return right + 1;
}

