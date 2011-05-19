#include <stdlib.h>
#include <stdio.h>

struct pair {
  int x;
  int* y; // Second item is pointer so that we can have it NULL
  struct pair* next;
};

typedef struct pair pair;

/*
  Searches the given list of pairs for value.

  Returns NULL if not found, pointer to pair struct otherwise.
*/
pair* findComplement(pair** pairs, int size, int value) {
  if (pairs[value%size] == NULL) {
    // Clearly not found
    return NULL;
  } else {
    // Otherwise, search the list at that index.
    pair* p = pairs[value%size];

    while (p != NULL) {
      if (p->x == value) return p;

      p = p->next;
    }
  }
}

/*
  Inserts a new pair struct into the list.
*/
void insertValue(pair** pairs, int size, int value) {
  int idx = value%size;

  if (pairs[idx] == NULL) {
    pairs[idx]       = malloc(sizeof(pair));
    pairs[idx]->x    = value;
    pairs[idx]->y    = NULL;
    pairs[idx]->next = NULL;
  } else {
    pair* p = pairs[idx];

    while (p->next != NULL)
      p = p->next;

    p->next       = malloc(sizeof(pair));
    p->next->x    = value;
    p->next->y    = NULL;
    p->next->next = NULL;
  }
}

pair** sumPairs(int* values, int size, int sum) {
  pair** pairs = malloc(sizeof(pair*) * size);
  int i;

  for (i = 0; i < size; i++) {
    // If the complement is not found, insert a new pair, otherwise
    // update the located pair with the values.
    pair* p = findComplement(pairs, size, sum-values[i]);
    if (p == NULL) {
      insertValue(pairs, size, values[i]);
    } else {
      p->y = malloc(sizeof(int));
      *(p->y) = values[i];
    }
  }

  return pairs;
}

int main() {
  int v[5] = {1, 2, 3, 4, 5};
  int sum = 3;
  int i;
  pair** pairs = sumPairs(v, 5, sum);

  for (i = 0; i < 5; i++) {
    if (pairs[i] != NULL) {
      pair* p = pairs[i];

      // TODO this really ought to be within sumPairs() and implemented as deleteValue()
      while (p != NULL) {
        if (p->y != NULL) printf("Found pair: %d,%d\n", p->x, *(p->y));
        p = p->next;
      }
    }
  }

  return 0;
}
