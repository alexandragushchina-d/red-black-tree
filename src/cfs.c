#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "cfs.h"
#include "rbtree.h"
#include "utility.h"

struct node* getLeft(struct node *root) {
  if (!root) {
    return NULL;
  }

  struct node *newNode = root;
  while (newNode->left) {
    newNode = newNode->left;
  }

  return newNode;
}

struct node* newRoot = NULL;

void createNewTree(struct node* n) {
  int newVal = n->val ;
  if (!newRoot) {
    newVal +=10 ;
  }

  newVal -= ((struct process*)n->data)->decay;

  struct node *newN = newNode(newVal);
  newRoot = insertRB(newRoot, newN);
  newN->data = n->data;
}

struct node* cfs(struct process *processes, int time) {
  if (!processes) {
    return NULL;
  }

  struct node *root = NULL;
  for (struct process *i = processes; i; i = i->next) {
    struct node *newN = newNode(i->PID);
    newN->data = i;
    root = insertRB(root, newN);
  }

  if (time == 0) {
    return root;
  }

  for (int i = 0; i < time; ++i) {
    struct node *leftNode = getLeft(root);
    printf("Time: %d PID: %d\n", leftNode->val, ((struct process*)leftNode->data)->PID);
    iterate_in_order(root, createNewTree);
    iterate_post_order(root, free_node);
    root = newRoot;
    newRoot = NULL;
  }

  return root;
}
