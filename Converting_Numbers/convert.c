#include <stdio.h>
#include <stdlib.h>

int main() {
	int decimal; //decimal variable
	int binary; //binary variable
	printf("Enter an integer : "); //ask for user input
	scanf("%d", &decimal); //grabs the user input
	printf("\n"); //new line
	printf("signed decimal   : %d\n", decimal); //signed decimal format
	printf("unsiged decimal  : %u\n", (unsigned int)decimal); //unsigned decimal format
	printf("hexadecimal      : %x\n", (unsigned int)decimal); //hexadecimal format
	printf("binary           : "); // binary format
	for(int i = 31; i >= 0; i--){ // for loop to go through the 32 bits
		binary = decimal >> i; // bit shift to the right by i
		if(i == 3 || i == 7 || i == 11 || i == 15 || i == 19 || i == 23 || i == 27){
			putc(' ', stdout); // put the space at every 4 spaces
		}
		if(binary & 1){ // if the bit we are looking iat is a 1 
			putc('1', stdout); // put it in the output
		}
		else{ //else
			putc('0', stdout); // put a 0
		}
	}
	printf("\n"); // new line to end the program
    return EXIT_SUCCESS;
}
