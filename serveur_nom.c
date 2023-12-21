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
#include "fonctions.h"

#define TAILLE 1024

int main(int argc , char * argv[]) 
{
	/*Verification arguments*/
	if(argc <= 0){
		fprintf(stderr, "An error occured\n");
		return EXIT_FAILURE;
	}
	
	/*Ouverture Fichier*/
	FILE * inputFile = fopen( argv[1], "r" );
	if ( inputFile == NULL ) {
        printf( "Cannot open file %s\n", argv[1] );
        exit( EXIT_FAILURE );
    }
    
    /*Initialisation socket*/
	int sock1;
	CHECK((sock1 = socket(AF_INET, SOCK_DGRAM, 0)) != -1);
	struct sockaddr_in new;
	struct sockaddr_in storage;
	socklen_t longeur;
	longeur = sizeof(struct sockaddr_in);
	memset((char *)&new,0,sizeof(new));
	new.sin_family = AF_INET;
	new.sin_port = htons(atol(argv[2]));
	new.sin_addr.s_addr = htonl(INADDR_ANY);
	CHECK(bind(sock1,(struct sockaddr *)&new,longeur) != -1);
    
    /*Initialisation socket6*/
	int sock6;
	CHECK((sock6 = socket(AF_INET6, SOCK_DGRAM, 0)) != -1);
	socklen_t l;
	l = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 serveur;
	struct sockaddr_in6 client;
	serveur.sin6_family = AF_INET6;
	serveur.sin6_port = htons(atol(argv[3]));
	serveur.sin6_addr = in6addr_any;
	CHECK(bind(sock6,(struct sockaddr *)&serveur,l) != -1);
	
	
	/*Structure storage*/
	char buf[TAILLE];
	memset(buf,'\0',1024);
	
	fd_set ensemble;
	
	while(1){
		memset(buf,'\0',1024);
		FD_ZERO(&ensemble);
		FD_SET(sock1,&ensemble);
		FD_SET(sock6,&ensemble);
		select(sock6+1, &ensemble, NULL, NULL, NULL);
		if(FD_ISSET(sock1, &ensemble)){	
			CHECK((recvfrom(sock1, buf, TAILLE, 0, (struct sockaddr *) &storage, &longeur)) > 0);
			printf("message = %s\n", buf);
			clientmessage c1;
			c1 = traiter_mess(buf);
			
			if (c1.id != "-1"){
				char * id = c1.id;
				char * horodatage = c1.horodatage;
				char * nom = malloc(128 * sizeof(char));
				strcpy(nom,c1.nom);
				int i = nb_occ(nom,'.');
				char *sous_domain = rech_sousdomaine(nom,i);
				printf("sousdomain : %s\n",sous_domain);
	
				/*Extraction adresse ligne pour le sous domaine*/
				char * message = creation_mess(inputFile,sous_domain,id,c1.nom,c1.horodatage);
		
				/*envoi du message*/
				CHECK((sendto(sock1, message, strlen(message), 0, (struct sockaddr *) &storage, longeur)) > 0);
				free(message);
			}
		}
		
		if(FD_ISSET(sock6, &ensemble)){
			CHECK((recvfrom(sock6, buf, TAILLE, 0, (struct sockaddr *) &client, &l)) > 0);
			printf("message = %s\n", buf);
			clientmessage c1;
			c1 = traiter_mess(buf);
			if (c1.id != "-1"){
				char * id = c1.id;
				char * horodatage = c1.horodatage;
				char * nom = malloc(128 * sizeof(char));
				strcpy(nom,c1.nom);
				int i = nb_occ(nom,'.');
				char *sous_domain = rech_sousdomaine(nom,i);
				printf("sousdomain : %s\n",sous_domain);
	
				/*Extraction adresse ligne pour le sous domaine*/
				char * message = creation_mess(inputFile,sous_domain,id,c1.nom,c1.horodatage);
		
				/*envoi du message*/
				CHECK((sendto(sock6, message, strlen(message), 0, (struct sockaddr *) &client, l)) > 0);
				free(message);
				free(nom);
			}
		}
	}
	
	/*Fermeture*/
	close(sock1);
	return EXIT_SUCCESS;
}
