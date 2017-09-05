#include <stdio.h>

int puts(char* s)
{
  for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
  return (putchar('\n') == '\n') ? 0 : EOF;
}
