#include<sys/defs.h>
#include<string.h>

char* strcat(char* d, const char* s)
{
	if(s == NULL || d == NULL)
		return NULL;
	char *ch = d;
	while(*ch != '\0') ch++;
	for(int i = 0; i < strlen(s); i++, ch++)
		*ch = s[i];
	*ch = '\0';
	return d;
}
