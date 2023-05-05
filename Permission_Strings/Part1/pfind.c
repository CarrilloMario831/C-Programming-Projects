#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>

// HEADS UP:
//
// Took Code from Dr. Borowski Below!

int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

char* permission_string(struct stat *statbuf) {
	char *perm_string;
	if ((perm_string = malloc(10 * sizeof(char))) == NULL) {
		fprintf(stderr, "Error: malloc failed. %s.\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < 9; i += 3) {
		// Using the ternary operator for succinct code.
		perm_string[i] = statbuf->st_mode & perms[i] ? 'r' : '-';
		perm_string[i + 1] = statbuf->st_mode & perms[i + 1] ? 'w' : '-';
		perm_string[i + 2] = statbuf->st_mode & perms[i + 2] ? 'x' : '-';
	}
	perm_string[9] = '\0';
	return perm_string;
}

int recurDir(char *path, char perm[]){
	
	DIR *dir;

	if ((dir = opendir(path)) == NULL) {
		fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", path, strerror(errno));
		return 0;
	}

	// All paths typically end in the name of the file, as in
	// /home/user.
	// Add 1 to PATH_MAX so we have room for a trailing '/' after the true
	// path name.
	
	char full_filename[PATH_MAX + 1];
	// Set the initial character to the NULL byte.
	// If the path is root '/', full_filename will be an empty string, and
	// we will need to take its strlen, so make sure it's null-terminated.
	full_filename[0] = '\0';
	if (strcmp(path, "/")) {
	// This code executes when the path is not the root '/'.
	// If there is no NULL byte in path, the full_filename will not be
	// terminated. So, take the minimum of the path length and PATH_MAX, and
	// copy the smaller amount of character into full_filename.
		size_t copy_len = strnlen(path, PATH_MAX);
		memcpy(full_filename, path, copy_len);
		full_filename[copy_len] = '\0';
	}
	// Add + 1 for the trailing '/' that we're going to add.
	size_t pathlen = strlen(full_filename) + 1;
	full_filename[pathlen - 1] = '/';
	full_filename[pathlen] = '\0';
	struct dirent *entry;
	struct stat sb;


	while ((entry = readdir(dir)) != NULL) {
		// Skip . and ..
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		// Add the current entry's name to the end of full_filename, following
		// the trailing '/' and overwriting the '\0'.
	
		strncpy(full_filename + pathlen, entry->d_name, PATH_MAX - pathlen);
		if (lstat(full_filename, &sb) < 0) {
			fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n", full_filename, strerror(errno));
			continue;
		}

		// Differentiate directories from other file types.
	
		if (S_ISDIR(sb.st_mode)) {
			char * perms = permission_string(&sb);

			if(strcmp(perms, perm) == 0){
				printf("%s\n", full_filename);	
			}
			free(perms);
			recurDir(full_filename, perm);
		}

		else {

			char * perms = permission_string(&sb);

                        if(strcmp(perms, perm) == 0){
                                printf("%s\n", full_filename);
                        }
			free(perms);
		}


	}


	closedir(dir);
	return 1;
	
}

int main(int argc, char **argv, char *envp[]) {
	int num_flag = 0;
	int d_flag = 0, p_flag = 0, h_flag = 0;
	int c;
	char *dirval = NULL;
	int argFourDash = 0;

	opterr = 0;

	if(strcmp(argv[1], "-d") == 0 && strcmp(argv[2], "-p") == 0){
                fprintf(stderr, "Error: Required argument -p <permission string> not found.\n");
                return EXIT_FAILURE;
        }
        else if(strcmp(argv[1], "-p") == 0 && strcmp(argv[2], "-d") == 0){
                fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
                return EXIT_FAILURE;
        }

	while((c = getopt(argc, argv, "dph")) != -1){
		
		switch(c) {
			case 'd':
				d_flag = 1;
				num_flag++;
				break;
			case 'p':
				p_flag = 1;
				num_flag++;
				break;
			case 'h':
				h_flag = 1;
				num_flag++;
				printf("Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
				return EXIT_SUCCESS;
				break;
			case '?':
				if(argv[4][0] == '-'){
					argFourDash++;
					break;
				}
				fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
				return EXIT_FAILURE;
				
			default:
				return EXIT_FAILURE;
		}
	}
	
	DIR *dir;

	char perm[10];

	if(num_flag > 3){
		fprintf(stderr, "Error: Too many flags specified.\n");
		return EXIT_FAILURE;
	}
	else if(d_flag == 1 && p_flag == 0 && h_flag == 0){
		fprintf(stderr, "Error: Required argument -p <permissions string> not found.\n");
		return EXIT_FAILURE;
	}
	else if(d_flag == 0 && p_flag == 1 && h_flag == 0){
		fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
		return EXIT_FAILURE;
	}
	else if(d_flag == 0 && p_flag == 0 && h_flag == 0){
		fprintf(stderr, "Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
		return EXIT_FAILURE;
	}

	else if(d_flag == 1 && p_flag == 1 && num_flag == 2 && argFourDash == 0){
		dirval = argv[3];
		if(dirval != NULL){
			dir = opendir(dirval);
			if(dir == NULL){
				fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", dirval, strerror(errno));
				closedir(dir);
		        	return EXIT_FAILURE;
			}
			else{	
				//this is where we check rwx
				if(strlen(argv[4]) != 9){
					fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[4]);
					closedir(dir);
					return EXIT_FAILURE;
				}
				else{
					for(int i = 0; i < 9; i++){
						char c = argv[4][i];
						if(c != 'r' && c != 'w' && c != 'x' && c != '-'){
							fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[4]);
							closedir(dir);
                                        		return EXIT_FAILURE;
						}

						if((i == 0 || i == 3 || i == 6) && (c != 'r' && c != '-')){
							fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[4]);
							closedir(dir);
                                                        return EXIT_FAILURE;
						}
						else if((i == 1 || i == 4 || i == 7) && (c != 'w' && c != '-')){
							fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[4]);
							closedir(dir);
                                                        return EXIT_FAILURE;
						}
						else if((i == 2 || i == 5 || i == 8) && (c != 'x' && c != '-')){
							fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[4]);
							closedir(dir);
                                                        return EXIT_FAILURE;
						}

					}
					//Given a good String, parse the directory
					for(int i = 0; i < 9; i++){
						perm[i] = argv[4][i];
					}
					perm[9] = '\0';
					
					recurDir(dirval,perm);

					closedir(dir);
				

					return EXIT_SUCCESS;
				}
                	}
		}
	}

	else if(d_flag == 1 && p_flag == 1 && num_flag == 2 && argFourDash != 0){
		dirval = argv[4];
                if(dirval != NULL){
                        dir = opendir(dirval);
                        if(dir == NULL){
                                fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", dirval, strerror(errno));
				closedir(dir);
                                return EXIT_FAILURE;
                        }
                        else{
                                //this is where we check rwx
                                if(strlen(argv[3]) != 9){
                                        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[3]);
					closedir(dir);
                                        return EXIT_FAILURE;
                                }
                                else{
                                        for(int i = 0; i < 9; i++){
                                                char c = argv[3][i];
                                                if(c != 'r' && c != 'w' && c != 'x' && c != '-'){
                                                        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[3]);
							closedir(dir);
                                                        return EXIT_FAILURE;
                                                }

                                                if((i == 0 || i == 3 || i == 6) && (c != 'r' && c != '-')){
                                                        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[3]);
							closedir(dir);
                                                        return EXIT_FAILURE;
                                                }
                                                else if((i == 1 || i == 4 || i == 7) && (c != 'w' && c != '-')){
                                                        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[3]);
							closedir(dir);
                                                        return EXIT_FAILURE;
                                                }
                                                else if((i == 2 || i == 5 || i == 8) && (c != 'x' && c != '-')){
                                                        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[3]);
							closedir(dir);
                                                        return EXIT_FAILURE;
                                                }

                                        }
                                        //Given a good String, parse the directory
					for(int i = 0; i < 9; i++){
                                        	perm[i] = argv[3][i];
                                        }
					perm[9] = '\0';

					recurDir(dirval,perm);

					closedir(dir);

                                        return EXIT_SUCCESS;
                                }
                        }
			closedir(dir);
                }
	}
	return EXIT_SUCCESS;
}
