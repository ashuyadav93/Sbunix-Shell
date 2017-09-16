#include <sys/defs.h>

size_t strlen(const char* s) {
   size_t len = 0;
   while(*s++ != '\0') {
      len++; 
   }
   return len;
}
