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

int nb_occ(char* chaine,char c)
{
    int i,nbc = 0,
    size = strlen(chaine);    
     
    for (i = 0 ; i < size ; i++)
    {
        if (chaine[i] == c) 
            nbc ++;         
    }
     
    return nbc;             
}

char * rech_domaine(char * buffer, int occ){
	
	char *token;
	char *saveptr;
	
	token = strtok_r(buffer, ".", &saveptr);
	
	for (int i=0; i<occ-1; i++){
		token = strtok_r(saveptr, ".", &saveptr);
	}
	
	return token;
}

int main(int argc , char * argv[]) 
{
	/*Verification arguments*/
	if(argc <= 0){
		fprintf(stderr, "An error occured\n");
		return EXIT_FAILURE;
	}
	
	/*Ouverture Fichier*/
	FILE * inputFile = fopen( "serveurfrip", "r" );
	if ( inputFile == NULL ) {
        printf( "Cannot open file %s\n", argv[0] );
        exit( EXIT_FAILURE );
    }
    
    /*Initialisation socket*/
	int sock1;
	CHECK((sock1 = socket(AF_INET, SOCK_DGRAM, 0)) != -1);
	
	/*Structure reception*/
	struct sockaddr_in new;
	socklen_t longeur;
	longeur = sizeof(struct sockaddr_in);
	memset((char *)&new,0,sizeof(new));
	new.sin_family = AF_INET;
	new.sin_port = htons(atol(argv[1]));
	new.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET, argv[2], &new.sin_addr);
	CHECK(bind(sock1,(struct sockaddr *)&new,longeur) != -1);
	
	/*Structure storage*/
	struct sockaddr_in storage;
	char buf[TAILLE];
	memset(buf,'\0',1024);
	size_t r1;
	CHECK((r1 = recvfrom(sock1, buf, TAILLE, 0, (struct sockaddr *) &storage, &longeur)) > 0);
	printf("message = %s\n", buf);
	
	/*Preparation strucutre d'envoi*/
	
	int sockfd;
	CHECK((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) != -1);
	
	/*Verification du nom de domain*/
	int i = nb_occ(buf,'.');
	char *domain = rech_domaine(buf,i);
	if (domain != "fr"){
		char *erreur = "Erreur nom non resolvable";
		ssize_t n;
		CHECK((n = sendto(sockfd, erreur, strlen(erreur), 0, (struct sockaddr *) &storage, longeur)) > 0);
	} else {
		
	
		/*EXtraction sous domain*/
		char * sous_domain = rech_domaine(buf, i-1);
	
		/*Extraction adresse ligne pour le sous domaine*/
		char * ligne = (char *) malloc(25);
		char * ip_port = (char *) malloc(25);
		while(fgets(ligne,25,inputFile) != NULL)
		{
		
			char * temp = strstr(ligne,sous_domain); 
			if(temp != NULL){
				strcpy(ip_port, ligne);
			}
		}
		free(ligne);	
	
		char *token;
		char *saveptr;
		char *ip_port_serv;

		token = strtok_r(ip_port, "|", &saveptr);
		ip_port_serv = saveptr;
		printf("serveur de domaine = %s\n", saveptr);
		
		/*envoi du message*/
		int sockfd;
		CHECK((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) != -1);
		struct sockaddr_in envoi = storage;
		ssize_t n;
		CHECK((n = sendto(sockfd, ip_port_serv, strlen(ip_port_serv), 0, (struct sockaddr *) &envoi, longeur)) > 0);
	}
	
	/*Fermeture*/
	close(sock1);
	close(sockfd);
	return EXIT_SUCCESS;
}
