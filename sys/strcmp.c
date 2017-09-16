#include<sys/defs.h>
#include<string.h>

int strcmp(const char *s, const char* t) {
    size_t a = strlen(s);
    size_t b = strlen(t);
    
    if(a > b) return 1;
    if(a < b) return -1;
    
    while(*s != '\0' && *t !='\0') {
      if(*s < *t) {
         return -1;
      } else if (*s > *t) {
         return 1;
      }
      s++;
      t++;
    }

    return 0;
}
