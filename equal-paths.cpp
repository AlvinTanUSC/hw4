#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node * root){
    if(root == nullptr)
      return 0;
    int h = 1;
    if(root->left != nullptr)
        h = max(h, height(root->left) + 1);
    if(root->right != nullptr)
        h = max(h, height(root->right) + 1);
    return h;
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr)
      return true;
    if(equalPaths(root->left) && equalPaths(root->right)){
      if(root->left == nullptr || root->right == nullptr)
        return true;
      else if (height(root->left) == height(root->right))
        return true;
      else
        return false;
    } else{
      return false;
    }
}

