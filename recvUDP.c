#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "check.h"

#define TAILLE 1024

int main(int argc , char * argv[]) 
{
	if(argc <= 0){
		fprintf(stderr, "An error occured\n");
		return EXIT_FAILURE;
	}
	
	int sock1;
	char buf[TAILLE];
	memset(buf,'\0',1024);
	struct sockaddr_in new;
	struct sockaddr_in storage;
	socklen_t longeur;
	longeur = sizeof(struct sockaddr_in);
	
	size_t r1;
	CHECK((sock1 = socket(AF_INET, SOCK_DGRAM, 0)) != -1);
	memset((char *)&new,0,sizeof(new));
	new.sin_family = AF_INET;
	new.sin_port = htons(atol(argv[1]));
	new.sin_addr.s_addr = htonl(INADDR_ANY);
	CHECK(bind(sock1,(struct sockaddr *)&new,longeur) != -1);
	
	
	CHECK((r1 = recvfrom(sock1, buf, TAILLE, 0, (struct sockaddr *) &storage, &longeur)) > 0);
	//printf("%ld\n",r1);
	printf("message = %s\n", buf);

	close(sock1);
	return EXIT_SUCCESS;
}
