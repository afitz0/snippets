#include <stdio.h>
#include <stdbool.h>

/*
  Returns true if target is a sub-string of src.
*/
bool isSubStr(const char* src, const char* target) {
  int i;

  if (src == NULL && target == NULL)
    return true;

  // Easy check: Are they simply the same string?
  if (strcmp(src, target) == 0)
    return true;

  for (i = 0; src[i] != '\0'; i++) {
    int j;
    bool match;
    
    for (j = 0; target[j] != '\0'; j++) {
      if (target[j] != src[i+j]) {
        match = false;
        break;
      } else {
        match = true;
      }
    }

    if (target[j] == '\0' && match) {
      return true;
    }
  }

  return false;
}

int main() {
  printf("'a' is a substr of 'aaa'?: %s\n", isSubStr("aaa", "a") ? "yes" : "no");
  printf("'aa' is a substr of 'aaa'?: %s\n", isSubStr("aaa", "aa") ? "yes" : "no");
  printf("'a' is a substr of 'b'?: %s\n", isSubStr("b", "a") ? "yes" : "no");

  printf("'aba' is a substr of 'baaba'?: %s\n", isSubStr("baaba", "aba") ? "yes" : "no");
  printf("'cat' is a substr of 'dog'?: %s\n", isSubStr("dog", "cat") ? "yes" : "no");
  printf("'cat' is a substr of 'acate'?: %s\n", isSubStr("acate", "cat") ? "yes" : "no");
}
