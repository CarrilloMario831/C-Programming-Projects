#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

void read_sort(FILE *fp, int intflag, int dblflag){
	char buf[MAX_ELEMENTS][MAX_STRLEN + 1];
	int i = 0;
	while(fgets(buf[i], MAX_STRLEN + 2, fp)){
		char *eoln = strchr(buf[i], '\n');
		if (eoln != NULL) {
			*eoln = '\0';
		}
		i++;
	}
	if(intflag == 1 && dblflag == 0){
		int arr[i];
		for(int j = 0; j < i; j++){
			arr[j] = atoi(buf[j]);
		}
		quicksort(arr, i, sizeof(int), int_cmp);
		for(int j = 0; j < i; j++){
			printf("%d\n", arr[j]);
		}
	}
	else if (dblflag == 1 && intflag == 0){
		double arr[i];
                for(int j = 0; j < i; j++){
                        arr[j] = atof(buf[j]);
                }
                quicksort(arr, i, sizeof(double), dbl_cmp);
                for(int j = 0; j < i; j++){
                        printf("%f\n", arr[j]);
                }
	}
	else if(intflag == 0 && dblflag == 0){
		char *arr[i];
		for(int j = 0; j < i;j++){
			arr[j] = buf[j];
		}
                quicksort(arr, i, sizeof(char *), str_cmp);
                for(int j = 0; j < i; j++){
                        printf("%s\n", arr[j]);
                }
	}
}

int main(int argc, char **argv) {
	int num_flag = 0;
	int intflag = 0, dblflag = 0;
	char *fileval = NULL;
	int c;

	opterr = 0;

	while((c = getopt(argc, argv, "id")) != -1){
		switch (c) {
            		case 'i':
                		intflag = 1;
				num_flag++;
                		break;
            		case 'd':
                		dblflag = 1;
				num_flag++;
                		break;
            		case '?':
                		fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
        			fprintf(stderr, "Usage: ./sort [-i|-d] [filename]\n");
        			fprintf(stderr, "   -i: Specifies the input contains ints.\n");
        			fprintf(stderr, "   -d: Specifies the input contains doubles.\n");
        			fprintf(stderr, "   filename: The file to sort. If no file is supplied, input is read from\n");
        			fprintf(stderr, "             stdin.\n");
        			fprintf(stderr, "   No flags defaults to sorting strings.\n");
                		return EXIT_FAILURE;
            		default:
                		return EXIT_FAILURE;
        	}
	}
	
	FILE *infile;

	if(argc == 2 && optind == 2){
		if(intflag == 1){
			read_sort(stdin, intflag, dblflag);
			return EXIT_SUCCESS;
		}
		else if(dblflag == 1){
			read_sort(stdin, intflag, dblflag);
			return EXIT_SUCCESS;
		}
	}
	else if(argc == 1){
		read_sort(stdin, intflag, dblflag);
		return EXIT_SUCCESS;
	}
	else if(argc - optind >= 2){
		fprintf(stderr, "Error: Too many files specified.\n");
		return EXIT_FAILURE;
	}
	else{
		fileval = argv[optind];
	}

	if(num_flag > 1){
		fprintf(stderr, "Error: Too many flags specified.\n");
		return EXIT_FAILURE;
	}
	else if(fileval != NULL){
		infile = fopen(fileval, "r");
	       	if(infile == NULL){
		       fprintf(stderr, "Error: Cannot open '%s'. %s.\n", fileval, strerror(errno));
		       return EXIT_FAILURE;
	       	}
		else{
			if(intflag == 1){
				read_sort(infile, intflag, dblflag);
				fclose(infile);
				return EXIT_SUCCESS;	
			}
			else if(dblflag == 1){
				read_sort(infile, intflag, dblflag);
				fclose(infile);
				return EXIT_SUCCESS;
			}
			else if(num_flag == 0){
				read_sort(infile, intflag, dblflag);
				fclose(infile);
				return EXIT_SUCCESS;
			}
		}	
	}
	return EXIT_SUCCESS;
}
