/**
Binary Search Tree

Provides the structures, accessors and mutators for a Binary Search Tree.
*/

struct bst_node {
  struct bst_node* parent;
  struct bst_node* left;
  struct bst_node* right;

  int value;
};

struct bst_node* bst_new() {
  struct bst_node* t = malloc(sizeof(struct bst_node));
  t->value    = 0;
  t->parent   = NULL;
  t->left     = NULL;
  t->right    = NULL;

  return t;
}

void bst_traverse(struct bst_node* root, void (*action)(const void* n)) {
  if (root->left != NULL)
    bst_traverse(root->left, action);

  action(root);

  if (root->right != NULL)
    bst_traverse(root->right, action);
}

void bst_insert_parent(int new_value, struct bst_node* root, struct bst_node* parent) {
  if (root == NULL) return;

  // Try insert left
  if (new_value <= root->value) {
    if (root->left == NULL) {
      struct bst_node* n = bst_new();
      n->value    = new_value;
      root->left  = n;
    } else
      bst_insert_parent(new_value, root->left, root);
  }
  
  // Otherwise try insert right
  else {
    if (root->right == NULL) {
      struct bst_node* n = bst_new();
      n->value    = new_value;
      root->right = n;
    } else
      bst_insert_parent(new_value, root->right, root);
  }
}

void bst_insert(int new_value, struct bst_node* root) {
  bst_insert_parent(new_value, root, NULL);
}
