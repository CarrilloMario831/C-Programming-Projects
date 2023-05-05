#include <stddef.h>
#include "mystring.h"

size_t my_strlen(char *src){
	size_t res = 0;
        if(src == NULL){
            return res;
        }
        else{
            while(*src != '\0'){
                res++;
                src = src + 1;
            }
        }
        return res;
}

char *my_strcpy(char *dst, char *src){
	char *res = dst;
	while(*src != '\0'){
		*dst = *src;
		src = src + 1;
		dst = dst + 1;
	}
	*dst = *src;
	return res;
}

void my_strlower(char *src){
	while(*src != '\0'){
		*src = tolower(*src);
		src = src + 1;	
	}
}
