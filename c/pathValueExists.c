/*
Given a binary tree, determine if there's a path that has the given value
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// A node of the binary tree
struct node {
  int value;
  struct node* left;
  struct node* right;
};

bool pathValueExists_real(struct node* tree, int value, int pathValue) {
  if (tree == NULL) {
    return false;
  }

  if (tree->left == NULL && tree->right == NULL) {
    if (tree->value+pathValue == value) {
      return true;
    } else {
      return false;
    }
  }

  pathValue = tree->value + pathValue;
  printf("pathValue now: %d\n", pathValue);
  return pathValueExists_real(tree->left, value, pathValue) ||
    pathValueExists_real(tree->right, value, pathValue);
}

bool pathValueExists(struct node* tree, int value) {
  pathValueExists_real(tree, value, 0);
}

// returns true if the value was inserted
bool insert_node(struct node* tree, int value) {
  bool inserted;

  if (tree == NULL) {
    return false;
  }

  if (tree->left == NULL) {
    tree->left = malloc(sizeof(struct node));
    tree->left->value = value;
    tree->left->left  = NULL;
    tree->left->right = NULL;
    return true;
  }
  
  if (tree->right == NULL) {
    tree->right = malloc(sizeof(struct node));
    tree->right->value = value;
    tree->right->left  = NULL;
    tree->right->right = NULL;
    return true;
  }

  // If we're here, we could actually insert a node, and need to recurse
  // down the tree
  if (value < tree->value) {
    return insert_node(tree->left, value);
  } else {
    return insert_node(tree->right, value);
  }
}

void print_tree(struct node* tree) {
  if (tree == NULL) return;

  printf("%d\n", tree->value);

  if (tree->left != NULL) {
    printf("\tleft child: %d\n", tree->left->value);
  }

  if (tree->right != NULL) {
    printf("\tright child: %d\n", tree->right->value);
  }

  print_tree(tree->left);
  print_tree(tree->right);
}

int main() {

  { // NULL case
    printf("0 exists in NULL? %s\n", pathValueExists(NULL, 0) ? "yes" : "no");
  }

  { // Singe node case
    struct node* t = malloc(sizeof(struct node));
    t->value = 0;
    t->left = NULL;
    t->right = NULL;

    printf("0 exists in t(0)? %s\n", pathValueExists(t, 0) ? "yes" : "no");
    printf("1 exists in t(0)? %s\n", pathValueExists(t, 1) ? "yes" : "no");
  }

  { // Case for multiple nodes
    int i = 0;
    struct node* tree = malloc(sizeof(struct node));
    tree->value = i;

    for (i = -5; i < 5; i++) {
      insert_node(tree, i);
    }

    print_tree(tree);

    printf("-8 exists in tree? %s\n", pathValueExists(tree, -8) ? "yes" : "no");
    printf("-4 exists in tree? %s\n", pathValueExists(tree, -4) ? "yes" : "no");
  }

  return 0;
}
