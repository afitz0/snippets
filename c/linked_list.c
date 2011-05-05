/**
  Basic implementation of a singly linked list.
*/
#include <stdio.h>
#include <stdlib.h>

struct node {
  int value;
  struct node* next;
};

// Create a new item set to value
struct node* new_node(int value) {
  struct node* list = malloc(sizeof(struct node));

  list->value = value;
  list->next = NULL;

  return list;
}

// Insert a new item into list l set to value.
struct node* insert_item(struct node* l, int value) {
  struct node* new;

  if (l != NULL) {
    struct node* n = l;
    while (n->next != NULL) {
      n = n->next; 
    }

    new = new_node(value);
    n->next = new;
  } else {
    new = new_node(value);
  }

  return new;
} 

// Print every item in list l, including the position at which it appears
void print_list(struct node* l) {
  struct node* n;
  int i;

  for (i = 0, n = l; n != NULL; i++, n = n->next) {
    printf("Item %d => %d\n", i, n->value);
  }
}

// Remove item with value from list l.
void remove_item(struct node* l, int value) {
  struct node* n = l;
  struct node* prev = NULL;

  while (n != NULL && n->value != value) {
    prev = n;
    n = n->next;
  }

  if (n != NULL) {
    prev->next = n->next;
  } else {
    prev->next = NULL;
  }

  free(n);
}

// Return a pointer to the item set to value
struct node* find_item(struct node* l, int value) {
  struct node* n = l;

  while (n != NULL && n->value != value) {
    n = n->next;
  }

  return n;
}

int main() {
  int i;

  struct node* list = new_node(0);
  printf("list's value: %d\n", list->value);

  for (i = 1; i < 100; i++) {
    insert_item(list, i);
  }
  print_list(list);

  puts("---------------------------------------------------------");

  {
    struct node* n = find_item(list, 50);
    if (n->value != 50) {
      printf("GOT WRONG ITEM; expected 50, got %d\n", n->value);
    } else {
      puts("Correctly retrieved item w/ value == 50");
    }
  }

  puts("---------------------------------------------------------");

  for (i = 25; i <= 75; i++) {
    remove_item(list, i);
  }
  print_list(list);
}
