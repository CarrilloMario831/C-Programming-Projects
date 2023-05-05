#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"

int client_socket = -1;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];


int handle_stdin() {
    int bytes_read, i = 0;
    char byte, temp[4096];
    bool too_long = false;
    while((bytes_read = read(STDIN_FILENO, &byte, sizeof(char))) > 0){ 	
	if(byte != '\n'){
	    if(byte ==  '\0'){
	        continue;
	    }
	    temp[i++] = byte;
	}
	if(i > MAX_MSG_LEN){
		if(too_long == false){
		    fprintf(stderr, "Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);
		}
		too_long = true;
        }
	if(byte == '\n'){
	    temp[i] = '\0';
	    
	    if(too_long == false){
		memcpy(outbuf, temp, i);    
	    	if(strcmp(outbuf, "") == 0){
	        	return EXIT_SUCCESS;
	    	}
            	if(send(client_socket, outbuf, strlen(outbuf) + 1, 0) < 0) {
                	fprintf(stderr, "Error: Failed to send message to server. %s.\n",strerror(errno));
                	return EXIT_FAILURE;
            	}
	    	if(strcmp(outbuf, "bye") == 0){
	        	printf("Goodbye.\n");
			return 3;
	    	}
	   	else{
			memset(outbuf, '\0', strlen(outbuf));
	        	return EXIT_SUCCESS;
	    	}
	    }
	    else{
	    	return EXIT_SUCCESS;
	    }


	}
	
    }
    if(bytes_read == 0){
	    return 3;
    }
    	    
    return EXIT_SUCCESS;
}

int handle_client_socket() {
    int bytes_recvd;
    if((bytes_recvd = recv(client_socket, inbuf, MAX_MSG_LEN, 0)) < 0) {
        fprintf(stderr, "Error: Failed to receive message from server. %s.\n",strerror(errno));
        return EXIT_FAILURE;
    }
    if (bytes_recvd == -1 && errno != EINTR) {
        fprintf(stderr, "Warning: Failed to receive incoming message.%s", strerror(errno));
    }
    else if(bytes_recvd == 0){
        fprintf(stderr, "\nConnection to server has been lost.\n");
	return EXIT_FAILURE;
    }
    else {
        // Process the incoming data. If "bye", the client disconnected.
	//strcat(outbuf, temp);
	//printf("%s\n", outbuf);	
	//printf("[%s]: %s\n", username,  inbuf);
	if(strcmp(inbuf, "bye") == 0){
	    printf("\nServer initiated shutdown.\n");
	    return 3;
	}
	printf("\n%s\n", inbuf);
	memset(inbuf, '\0', strlen(inbuf));
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    if(argc != 3){
    	fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
	return EXIT_FAILURE;
    }
    bool fileR = false;
    //int client_socket, bytes_recvd, ip_conversion, retval = EXIT_SUCCESS;
    int bytes_recvd, ip_conversion, retval = EXIT_SUCCESS;
    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    char *addr_str = argv[1]; 

    memset(&server_addr, 0, addrlen); 

    // Convert character string into a network address.
    ip_conversion = inet_pton(AF_INET, addr_str, &server_addr.sin_addr);

    if (ip_conversion == 0) {
	    fprintf(stderr, "Error: Invalid IP address '%s'.\n", addr_str);
	    retval = EXIT_FAILURE;
	    goto EXIT;
    } else if (ip_conversion < 0) {
	    fprintf(stderr, "Error: Failed to convert IP address. %s.\n", strerror(errno));
	    retval = EXIT_FAILURE;
	    goto EXIT;
    }

    int port;

    if(parse_int(argv[2], &port, "port number") == false){
	retval = EXIT_FAILURE;
	goto EXIT;
    }
    else{
    	if(port < 1024 || port > 65535){
		fprintf(stderr, "Error: Port must be in range [1024, 65535].\n");
		retval = EXIT_FAILURE;
		goto EXIT;
	}
    }

    server_addr.sin_family = AF_INET;   // Internet address family
    server_addr.sin_port = htons(port);

    if(isatty(STDIN_FILENO) == 1){

    	bool get_name = true;    

    	while(get_name){

    		printf("Enter your username: ");

		fflush(stdout);

		char buf[4096];

		if(fgets(buf, sizeof(buf), stdin) == NULL){
			if (errno == EINTR) {
                                	printf("\n");
                                	errno = 0;
                                	continue;
			} else if (feof(stdin)) {
                                	printf("\n");
                                	return EXIT_SUCCESS;
			} else if (ferror(stdin)) {
                                	printf("\n");
					return EXIT_FAILURE;
                	}
		}

		if(strlen(buf) - 1 > MAX_NAME_LEN){
			printf("Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
			for(int k = 0; k < strlen(buf); k++){
                            	buf[k] = '\0';
			}
                	continue;
		}

		memcpy(username, buf, sizeof(username));

		char *eoln = strchr(username, '\n');

		if (eoln != NULL) {
			*eoln = '\0';
		}

		if(strcmp(username, "")){

			printf("Hello, %s. Let's try to connect to the server.\n", username);
			get_name = false;

		}
    	}
	    
    }

    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
           fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
           retval = EXIT_FAILURE;
           goto EXIT;
    }
    if(connect(client_socket, (struct sockaddr *) &server_addr, addrlen) < 0){
        fprintf(stderr, "Error: Failed to connect to server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    if((bytes_recvd = recv(client_socket, inbuf, BUFLEN - 1, 0)) < 0) {
        fprintf(stderr, "Error: Failed to receive message from server. %s.\n",strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    printf("\n%s\n\n", inbuf);
    
    
    if(send(client_socket, username, strlen(username) + 1, 0) < 0) {
        fprintf(stderr, "Error: Failed to send message to server. %s.\n",strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    bool working = true;
    fd_set sockset;
    while(working){
	if(fileR == false){
	    printf("[%s]: ", username);
    	    fflush(stdout);
	}    
        FD_ZERO(&sockset);
	FD_SET(STDIN_FILENO, &sockset);
        FD_SET(client_socket, &sockset);
        if(select(client_socket + 1, &sockset, NULL, NULL, NULL) < 0 && errno != EINTR) {
            fprintf(stderr, "Error: select() failed. %s.\n", strerror(errno));
            retval = EXIT_FAILURE;
            goto EXIT;
        }
	if(FD_ISSET(STDIN_FILENO, &sockset)){
	    int val = handle_stdin();
	    if(val == EXIT_FAILURE){
	        retval = EXIT_FAILURE;
		goto EXIT;
	    }
	    else if(val == 3){
	        retval = EXIT_SUCCESS;
		printf("\n");
		goto EXIT;
	    }
	}

	if(FD_ISSET(client_socket, &sockset)){
            int val = handle_client_socket();
	    if(val == EXIT_FAILURE){
                retval = EXIT_FAILURE;
                goto EXIT;
            }
	    else if(val == 3){
                retval = EXIT_SUCCESS;
		goto EXIT;
            }
        }

    }

    EXIT:
    // If client socket has a file descriptor, close it.
    if (fcntl(client_socket, F_GETFD) != -1) {
	    close(client_socket);
    }
    return retval;

}
