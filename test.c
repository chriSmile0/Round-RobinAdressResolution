#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "check.h"
#include "fonctions.h"

#define taille 5

char * message_client(char * nom, char * id, char * horo){
	
	char * message =  malloc (1024 * sizeof(char));
	memset(message,'\0',1024);
	strcat(strcat(strcat(strcat(strcat(message,id),"|"),horo),"|"),nom);
	
	return message;
	
}

int main(int argc,char *argv[])
{
	char * nom = "www.eric.fr";
	char * id = "1";
	char horo[20];
	time_t timestamp = time( NULL );
	snprintf(horo, 20, "%ld", timestamp);
	char * message = message_client(nom,id,horo);
	printf("message = %s\n",message);

	return EXIT_SUCCESS;
}
