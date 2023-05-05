/**
 * Homework 2 Part 1 - Team 13
 * Name: Mario Carrillo
 * uni: mc5132
 * version 1: 02/09/23          
 */

#include <stdio.h>
#include <stdlib.h>
#include "iterative.h"
#include "recursive.h"

int main(int argc, char **argv) {
    	
	if(argc != 3){
		fprintf(stderr, "Usage: ./gcd <integer m> <integer n>\n");
		return EXIT_FAILURE;
	}
	else{
		if((atoi(argv[1]) == 0) && (atoi(argv[2]) == 0)){
			printf("gcd(%s, %s) = undefined\n", argv[1], argv[2]);
			return EXIT_SUCCESS;
		}
		else{
			int m = atoi(argv[1]), n = atoi(argv[2]);

			printf("Iterative: gcd(%d, %d) = %d\n", m, n, gcd_iterative(m, n));
			printf("Recursive: gcd(%d, %d) = %d\n", m, n, gcd_recursive(m, n));
			return EXIT_SUCCESS;
		}
	}
}
