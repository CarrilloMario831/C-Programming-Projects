#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void print_list(linked_list *list, void (*print_function)(void*)) {
    putchar('[');
    node *cur = list->head;
    if (cur != NULL) {
        print_function(cur->data);
        cur = cur->next;
    }
    for ( ; cur != NULL; cur = cur->next) {
        printf(", ");
        print_function(cur->data);
    }
    printf("]\n{length: %lu, head->data: ", list->size);
    list->head != NULL ? print_function(list->head->data) :
                         (void)printf("NULL");
    printf(", tail->data: ");
    list->tail != NULL ? print_function(list->tail->data) :
                         (void)printf("NULL");
    printf("}\n\n");
}

linked_list* create_linked_list(){
	linked_list* res = (linked_list *)malloc(sizeof(linked_list));
	//create a link list pointer and allocate memory to it
	if(res != NULL){
		//if the linklist is null
		res->head = NULL;
		//set the head to null
		res->tail = NULL;
		//set the tail to null
		res->size = 0;
		//set the size to 0
	}
	return res;
	//return the list
}

bool insert_in_order(linked_list *list, void *data, int (*cmp)(const void*, const void*)){
	if(list == NULL){
		//if the list is null
		return 0;	
		//return 0
	}
	if(list->head == NULL){
		//if the head is null
		list->head = create_node(data);
		//create a node with the data
		list->tail = list->head;
		//set the tail as the head since there is only one value
		list->size++;
		//increase the size of the link list
		return 1;
		//return 1
	}
	node *cur = list->head;
	//create a current node pointer
	node *prior;
	//create a prior node pointer
	node *n = create_node(data);
	//create a node with the data we are adding
	if(cmp(cur->data, data) <= 0){
		//if the value we are looking at is less than the data we want to insert
		while(cur != NULL && cmp(cur->data, data) < 0){
			prior = cur;
			cur = cur->next;
			//increment the pointer till one before insertion
		}
		if(prior->next != NULL){
			prior->next->prev = n;
			n->next = prior->next;
			prior->next = n;
			n->prev = prior;
			list->size++;		
		}
		else{
			prior->next = n;
			n->prev = prior;
			list->tail = n;
			list->size++;
		}
				
		return 1;	
	}
	else if(cmp(cur->data, data) > 0){
		n->next = cur;
		cur->prev = n;
		list->head = n;
		list->size++;
		return 1;
	}
	return 1;
}

void free_list(linked_list *list, void (*free_data)(void *)){
	node *n = list->head;
	node *t;
	while(n != NULL){
		t = n;
		n = n->next;
		free_node(t, free_data);
	}
	free(list);
}
