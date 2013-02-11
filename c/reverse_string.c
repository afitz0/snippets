#include <stdio.h>

void reverse(char*);

int main() {
  {
    char s[3] = "bc";
    printf("%s reversed: ", s);
    fflush(stdout);
    reverse(s);
    printf("%s\n", s);
  }

  {
    char s[4] = "abc";
    printf("%s reversed: ", s);
    fflush(stdout);
    reverse(s);
    printf("%s\n", s);
  }

  {
    char s[10] = "1234567890";
    printf("%s reversed: ", s);
    fflush(stdout);
    reverse(s);
    printf("%s\n", s);
  }
}

/*
Mistakes:
  - Forgot to increment first and was incrementing instead of decrementing last
  - First draft does not terminate.
  -- Didn't dereference last for checking for \0
  - Next run seg faulted on "*first = *last"
  -- This was *not* due to my reverse function, but rather how I was initializing the character strings for tests.

Differences from book's approach:
  - use while loop for incrementing end
  - no need to maintain seperate pointer for first character (since caller still has handle on that)
  - inline decrement and increment of first/last
  -- I don't like this as it's harder to read quickly.
*/
void reverse(char* str) {
  if (str == '\0') return;

  char* first = str;
  char* last = str;
  for (last; *last != '\0'; last++);
  last--;

  while (last > first) {
    char tmp = *first;
    *first = *last;
    *last = tmp;
    last--; first++;
  }

  return;
}

void book_reverse(char *str) {
  char* end = str;
  char tmp;
  if (str) {
    while (*end) {
      ++end;
      }
      --end;

      while (str < end) {
        tmp = *str;
        *str++ = *end;
        *end-- = tmp;
      }
    }
}
