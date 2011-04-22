#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bst.h"

void visit_action(const void* _n) {
  const struct bst_node* n = (const struct bst_node*)_n; 
  printf("%d\n", n->value);
}

int main() {
  int i, v;

  srandom(time(NULL));

  struct bst_node* tree = bst_new();
  v = random() % 1000;
  tree->value = v;

  for (i = 0; i < 1000000; i++) {
    v = random() % 1000;

    bst_insert(v, tree);
  }

  bst_traverse(tree, visit_action);
}
