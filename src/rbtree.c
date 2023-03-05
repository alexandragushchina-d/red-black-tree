#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rbtree.h"

struct node *newNode(int val) {
  struct node *res = (struct node*) malloc(sizeof(struct node));
    if (res) {
        res->val = val;
        res->color = RED;
        res->data = NULL;
        res->left = NULL;
        res->right = NULL;
        res->parent = NULL;
    }
  return res;
}

struct node *rotateLeft(struct node *root, struct node *n) {
  if(root == NULL) {
    return NULL;
  }
  if(n == NULL || n->right == NULL) {
    return root;
  }
  struct node *res = n->right;
  n->right = res->left;
  res->left = n;
  if (n->right) {
    n->right->parent = n;
  }

  res->parent = n->parent;
  n->parent = res;
  if (res->parent) {
    if (res->parent->left == n) {
      res->parent->left = res;
    } else {
      res->parent->right = res;
    }
  }

  return res;
}

struct node *rotateRight(struct node *root, struct node *n) {
  if(root == NULL) {
    return NULL;
  }
  if(n == NULL || n->left == NULL) {
    return root;
  }
  struct node *res = n->left;
  n->left = res->right;
  res->right = n;
  if (n->left) {
    n->left->parent = n;
  }

  res->parent = n->parent;
  n->parent = res;
  if (res->parent) {
    if (res->parent->left == n) {
      res->parent->left = res;
    } else {
      res->parent->right = res;
    }
  }

  return res;
}

struct node *getUncle(struct node *n) {
  struct node *grandparent = n->parent->parent;
  if (!grandparent) {
    return NULL;
  }

  if (n->parent == grandparent->left) {
    return grandparent->right;
  } else {
    return grandparent->left;
  }
}

struct node *rebalance(struct node *pnode, struct node *rnode) {
  if (rnode == NULL) {
    return pnode;
  }

  if (rnode->color == BLACK) {
    return rebalance(rnode, rnode->parent);
  }

  if (!rnode->parent) {
    rnode->color = BLACK;
  } else if (rnode->parent->color == RED) {
    struct node *uncle = getUncle(rnode);
    if (uncle && uncle->color == RED) {
      rnode->parent->color = BLACK;
      uncle->color = BLACK;
      rnode->parent->parent->color = RED;
    } else if (rnode->parent->parent) {
      if (rnode == rnode->parent->right &&
          rnode->parent == rnode->parent->parent->left) {
        rotateLeft(rnode->parent, rnode->parent);
        rnode = rnode->left;
      } else if (rnode == rnode->parent->left &&
                 rnode->parent == rnode->parent->parent->right) {
        rotateRight(rnode->parent, rnode->parent);
        rnode = rnode->right;
      }

      rnode->parent->color = BLACK;
      rnode->parent->parent->color = RED;
      if (rnode == rnode->parent->left && rnode->parent == rnode->parent->parent->left) {
        rotateRight(rnode->parent->parent, rnode->parent->parent);

      } else {
        rotateLeft(rnode->parent->parent, rnode->parent->parent);
      }
    }
  }

  return rebalance(rnode, rnode->parent);
}

struct node *insertRB(struct node *root, struct node *newNode) {
  if (newNode == NULL) {
    return root;
  }

  struct node *parent = root;
  while (parent) {
    if (parent->val > newNode->val && parent->left) {
      parent = parent->left;
      continue;
    }

    if (parent->val < newNode->val && parent->right) {
      parent = parent->right;
      continue;
    }

    break;
  }

  if (parent && parent->val == newNode->val) {
    return root;
  }

  if (parent && parent->val > newNode->val) {
    parent->left = newNode;
    newNode->parent = parent;
  } else if (parent && parent->val < newNode->val) {
    parent->right = newNode;
    newNode->parent = parent;
  }

  root = rebalance(NULL, newNode);
  return root;
}
