#include <stdio.h>
#include <libc.h>

int putchar(char c)
{
  write(1, &c, 1);
  return c;
}
