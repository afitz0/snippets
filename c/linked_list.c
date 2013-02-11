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

// Print every item in list l
void print_list(struct node* l) {
  struct node* n = l;
  int i;

  printf("[");
  for (i = 0; n != NULL; i++, n = n->next) {
    printf("'%d'", n->value);
    if (n->next != NULL) printf(",");
    printf(" ");
  }
  printf("]\n");
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

// reverse the order of the list such that 1 -> 2 -> 3 becomes 3 -> 2 -> 1
struct node* reverse_list(struct node* l) {
  struct node* new_head = l;

  if (new_head == NULL) {
    return new_head;
  }

  struct node* next = new_head->next;
  new_head->next = NULL;

  while (next != NULL) {
    struct node* tmp = next->next;
    next->next = new_head;

    new_head = next;
    next = tmp;
  }

  return new_head;
}

// swap pairs of the list such that 1 -> 2 -> 3 -> 4 -> 5 becomes 2 -> 1 -> 4 -> 3 -> 5
struct node* swap_pairs(struct node* l) {
  struct node* new_head = l;

  if (new_head == NULL) {
    return new_head;
  }

  // TODO implement me.
}

// Given a node, change its next to next->next
void removeNext(struct node* n) {
  if (n == NULL) return;
  if (n->next != NULL) {
    struct node* newNext = n->next->next;
    n->next = newNext;
  }
  return;
}

// O(n^2) imnplementation of removing duplicates
struct node* removeDups(struct node* list) {
  if (list == NULL || list->next == NULL) return list;

  struct node* curr = list;
  while (curr != NULL) {
    struct node* prev = curr;
    struct node* lookAhead = curr->next;
    while (lookAhead != NULL) {
      if (lookAhead->value == curr->value) {
        removeNext(prev);
        lookAhead = prev->next;
      } else {
        lookAhead = lookAhead->next;
        prev = prev->next;
      }
    }
    curr = curr->next;
  }
  return list;
}

int main() {
  int i;

  // Generally these are required to test anything.
  puts("Add single node.");
  struct node* list = new_node(0);
  printf("\tnode's value: %d\n", list->value);

  puts("Add lots of nodes.");
  for (i = 1; i < 100; i++) {
    insert_item(list, i);
  }
  print_list(list);
  // ---- end initialization of list ----

  puts("Adding some dupes");
  for (i = 0; i < 10; i++) {
    insert_item(list, i);
  }

  puts("Remove duplicates of list.");
  removeDups(list);
  puts("New list:");
  print_list(list);
}
