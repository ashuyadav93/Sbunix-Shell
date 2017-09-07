#include<sys/defs.h>
#include<string.h>

char* strcpy(char* d, const char* s)
{
	if(s == NULL || d == NULL)
		return NULL;
	int i;
	int len = strlen(s);
	for(i = 0; i < len; i++)
		d[i] = s[i];
	d[i] = '\0';
	return d;
}
