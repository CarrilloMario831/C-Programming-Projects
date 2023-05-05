#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char *argv[]){

	if(argc != 5){
		fprintf(stderr, "Usage: ./spfind -d <directory> -p <permissions string> [-h]\n");
		return EXIT_FAILURE;
	}
	else{
		int pfind_to_sort[2], sort_to_spfind[2];
		pipe(pfind_to_sort);
		pipe(sort_to_spfind);

		pid_t pid[2];

		if ((pid[0] = fork()) == 0) {
			
			close(pfind_to_sort[0]);
			dup2(pfind_to_sort[1], STDOUT_FILENO);
			close(pfind_to_sort[1]);

			close(sort_to_spfind[0]);
			close(sort_to_spfind[1]);

			if(execlp("./pfind", "./pfind", argv[1], argv[2], argv[3], argv[4], NULL) == -1){
				fprintf(stderr, "Error: pfind failed.\n");
				return EXIT_FAILURE;
			}
		}
		else if(pid[0] == -1){
			fprintf(stderr, "Fork Failed\n");
			return EXIT_FAILURE;
		}
		
		if((pid[1] = fork()) == 0){
			close(pfind_to_sort[1]);
			dup2(pfind_to_sort[0], STDIN_FILENO);
			close(pfind_to_sort[0]);
			close(sort_to_spfind[0]);
			dup2(sort_to_spfind[1], STDOUT_FILENO);
			close(sort_to_spfind[1]);

			if(execlp("sort", "sort", NULL) == -1){
				fprintf(stderr, "Error: sort failed.");
				return EXIT_FAILURE;
			}

		}
		else if(pid[1] == -1){
			fprintf(stderr, "Fork Failed\n");
			return EXIT_FAILURE;
		}

		close(pfind_to_sort[0]);
		close(pfind_to_sort[1]);
		close(sort_to_spfind[1]);
		
		char buf[PATH_MAX];
		int ct = 0;
					
		ssize_t count;
		while((count = read(sort_to_spfind[0], buf, sizeof(buf) - 1))){
			if (count == -1) {
                        	perror("read()");
                        	exit(EXIT_FAILURE);
                	}
			buf[count] = '\0';

			if(write(STDOUT_FILENO, buf, count) == -1){
				fprintf(stderr, "Write Failed\n");
				return EXIT_FAILURE;
			}
			int i = 0;
			
			while(i < count){
				if(buf[i] == '\n'){
					ct++;		
				}
				i++;
			}

		}
		
		close(sort_to_spfind[0]);

		int status;

		if(wait(&status) == -1){
			fprintf(stderr, "Wait Failed\n");
			return EXIT_FAILURE;
		}

		if(wait(NULL) == -1){
			fprintf(stderr, "Wait Failed\n");
			return EXIT_FAILURE;
		}

		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
        		printf("Total Matches: %d\n", ct);
        		return EXIT_SUCCESS;
    		}

		return EXIT_SUCCESS;
	}
}
