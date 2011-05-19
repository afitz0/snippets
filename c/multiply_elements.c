/*
  Given an array 'A' of numbers, generate array 'Output' such that 
  Output[i] = ( A[0] * A[1] ... * A[n] ) / A[i]

  Do it in O(n), and as a bonus, do it without divide.
*/
#include <stdlib.h>
#include <stdio.h>

#define ITEMS 10

int main() {
  long Output[ITEMS], total;
  int i;

  long A[ITEMS] = {90, 50, 4, 36, 35, 52, 51, 64, 24, 4};

  total = 1;
  for (i = 0; i < ITEMS; i++) {
    total *= A[i];
  }

  for (i = 0; i < ITEMS; i++) {
    Output[i] = total / A[i];
    printf("Output[%d] := %ld\n", i, Output[i]);
  }
}
