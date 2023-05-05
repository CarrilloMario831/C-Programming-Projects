#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>

#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT    "\x1b[0m"

/* signal_val is not used in this project but is shown for completeness. */
volatile sig_atomic_t signal_val = 0;

/* Signal handler */
void catch_signal(int sig) {
	signal_val = sig;
}

int cd(char *path){
	if(path == NULL || strcmp(path, "~") == 0){
		uid_t uid = getuid();
		struct passwd *pw = getpwuid(uid);
		if(pw == NULL){
			fprintf(stderr, "Error: Cannot get paswd entry. %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		path = pw->pw_dir;
	}
	return chdir(path);
}

int shell_exit(){
	exit(EXIT_SUCCESS);
}

int main(){

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = catch_signal;

	if (sigaction(SIGINT, &action, NULL) == -1) {
		fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	}

	char buf[4096];

	while(1){

		//Create the minishell text [pwd]$
		char command[4096];
        	getcwd(command, sizeof(command));
        	printf("[%s%s%s]$ ", BRIGHTBLUE, command, DEFAULT);

		fflush(stdout);


		if (fgets(buf, sizeof(buf), stdin) == NULL) {
                        if (errno == EINTR) {
                                printf("\n");
                                errno = 0; // Need to reset errno to 0. fgets() does not do
                               		// this for us.
                                continue;
                        } else if (feof(stdin)) {
                                printf("\n");
                                return EXIT_SUCCESS;
                        } else if (ferror(stdin)) {
                                printf("\n");
                                return EXIT_FAILURE;
                        }
                }

		char *eoln = strchr(buf, '\n');

                if (eoln != NULL) {
                 	*eoln = '\0';
                }
		
		if(strcmp(buf, "")){

			char *cmd = strtok(buf, " ");
			char *argv[4096];
			int argc = 0;

			while(cmd != NULL){
				argv[argc++] = cmd;
				cmd = strtok(NULL, " ");
			}
			argv[argc] = NULL;
		
			if(strcmp(argv[0], "cd") == 0){
				if(cd(argv[1]) == -1){
					fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", argv[1], strerror(errno));
				}
			}
			else if(strcmp(argv[0], "exit") == 0){
				shell_exit();
			}
			else{
				pid_t pid;
				if((pid = fork()) == 0){
					if(execvp(argv[0], argv) == -1){
						fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
						return EXIT_FAILURE;
					}
				}
				else if(pid == -1){
					fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
					return EXIT_FAILURE;
				}
				else{
					if(waitpid(pid, NULL, 0) == -1){
						printf("\n");
					}
				}
			}
		}

	}

	return EXIT_SUCCESS;
}
