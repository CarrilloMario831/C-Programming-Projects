#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"

/**
 * Take an array of char* pointers and print each of the strings to standard
 * out. This function must use only pointer arithmetic and no array
 * subscripting. The output start with "[", ends with "]\n", and prints the
 * strings inside, each separated by a comma and space.
 * Example: [Hi, BYE, AP, COMSW 3157, FunTimes]
 */
void display_strings(char **strings) {
    printf("[");
    if (strings != NULL) {
        char **str_ptr = strings;
        if (*str_ptr != NULL) {
            printf("%s", *str_ptr++);
            while (*str_ptr != NULL) {
                printf(", %s", *str_ptr++);
            }
        }
    }
    printf("]\n");
}

static char **copy_args_lowercase(int argc, char **argv) {
	char **res = (char **) malloc(argc * sizeof(char *));
	if(argc == 1){
		return NULL;
	}
	int i = 0;
	for(; i < argc - 1; i++){
		res[i] = (char *) malloc((sizeof(char) * (my_strlen(argv[i + 1]) + 1)));
		res[i] = my_strcpy(res[i], argv[i + 1]);
		my_strlower(res[i]);
	}
	res[i] = NULL;
	return res;

}

static void free_copy(char **copy) {
	if(copy == NULL){
		return;
	}
	char **res = copy;
    	while(*res != NULL){
		free(*res);
		res++;
	
	}
	free(copy);
}


/**
 * DO NOT MODIFY main()
 */
int main(int argc, char **argv) {
    char **copy;
    copy = copy_args_lowercase(argc, argv);

    printf("Raw arguments       : ");
    display_strings(argv + 1);

    printf("Lowercase arguments : ");
    display_strings(copy);

    free_copy(copy);

    return EXIT_SUCCESS;
}
