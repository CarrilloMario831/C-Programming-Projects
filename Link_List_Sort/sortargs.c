#include <stdio.h>
#include <string.h>
#include "linked_list.h"

void print(void *data){
	printf("\"%s\"", (char*) data);
}

void free_empty(void *data){
	return;
}

int main(int argc, char **argv) {

	linked_list* list = create_linked_list();
	
	print_list(list, print);

	for(int i = 0; i < argc-1; i++){
		insert_in_order(list, argv[i + 1], (int (*) (const void*, const void*)) strcmp);
		print_list(list, print);
	}

	free_list(list, free_empty);

    return EXIT_SUCCESS;
}
