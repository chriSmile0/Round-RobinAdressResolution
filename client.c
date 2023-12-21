#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "check.h"
#include "fonctions.h"

#define taille 1024
#define taille_cache 30

// NB : - commentaires du 25/05 (un peu tard dans la soirée donc voir si cohérence)



int main(int argc , char * argv[]) 
{
	/*Verification arguments*/
	if(argc <= 0){
		fprintf(stderr, "An error occured\n");
		return EXIT_FAILURE;
	}
	
	/*ouverture | lecture fichier*/
	int fd;
	CHECK((fd = open("clientip", O_RDONLY)) != -1);
	char buffer[taille];
	memset(buffer, '\0', taille);
	
	while(read(fd, (char *)buffer, taille) > 0 ){
		printf("serveur racine = %s\n", buffer);
	}
	
	/*Decomposition adresse ip port*/
	char *token;
	char *rest;
	char *ip_1;
	char *port1;
	char *saveptr;
	char *saveptr1;
	token = strtok_r(buffer, "|", &saveptr);
	ip_1 = token;
	printf("ip = %s\n", ip_1);
	token = strtok_r(saveptr, "|", &saveptr1);
	port1 = token;
	printf("port = %s\n", port1);
	
	/*initialisation socket d'envoi*/
	int sockfd;
	CHECK((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) != -1);
	
	/*Structure d'envoi ipv4*/
	socklen_t longeur = sizeof(struct sockaddr_in);
	struct sockaddr_in new;
	memset((char *)&new,0,sizeof(new));
	/*new.sin_family = AF_INET;
	new.sin_port = htons(atol(port1));
	inet_pton(AF_INET, ip_1, &new.sin_addr);*/

	/*Structure d'envoie ipv6 */
	socklen_t longeur6 = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 new6;
	memset((char *)&new6,0,sizeof(new6));
	new6.sin6_family = AF_INET6;
	new6.sin6_port = htons(atol(port1));
	char * ip_6 = "::1";
	inet_pton(AF_INET6, ip_6, &new6.sin6_addr);

	struct cache_resolution *new_c_r;
	new_c_r = malloc(taille_cache*sizeof(struct cache_resolution));//une bonne taille car on ne va pas recevoir que le buf
	init_cache_resolution(new_c_r,taille_cache);//same

	char buf[taille];
	memset(buf,'\0',1024);
	size_t r1;
	char tab_buffer[30][100];//30 chaines de taille 100 max 
	int goon = 1;
	fd_set ens,tmp;
	int *tableau_sock_fd;
	tableau_sock_fd = malloc(30*sizeof(int));//pour le moment taille 30
	int f = -10;
	struct timeval delai;
	struct timeval d;
	delai.tv_sec = 1;
	delai.tv_usec = 0;

	char * message = "1|1685395659|zombie.eric.fr";
	char * message2 = "4|168304404|zombie.eric.fr";
	char * message3 = "7|122222222|zombie.eric.fr";
	char * message4 = "10|19999999|example.eric.fr";
	char * message5 = "15|3000000|chris.moodle.fr";
	char * message6 = "18|9000000|toto.vs.com";
	int goo = 1;
	int cpt_mot = 0;//Nombre de requetes que l'on aura recueillis du fichier

	while (goo) {
	/*Envoi du message*/
		new.sin_family = AF_INET;
		new.sin_port = htons(atol(port1));
		inet_pton(AF_INET, ip_1, &new.sin_addr);
		char * saveptrid = malloc(1024*sizeof(char));
		strncpy(saveptrid,message,strlen(message));
		char * id_p = strtok_r(saveptrid,"|",&saveptrid);
		int id_principal = extraction_first_id_chaine(id_p);
		char * new_argv2 = change_first_id_chaine(message,id_principal);
		char * new_argv3 = change_horo_chaine(new_argv2,time(NULL));
		ssize_t n;
		CHECK((n = sendto(sockfd, new_argv3, strlen(new_argv3), 0, (struct sockaddr *) &new, longeur)) > 0);
		printf("%s\n", new_argv3);

		/*Structure de reception*/
		memset(buf,'\0',taille);
		CHECK((r1 = recvfrom(sockfd, buf, taille, 0, (struct sockaddr *) &new, &longeur)) > 0);
		printf("%s\n", buf);

		int nb_domaine = nb_occ(new_argv3,'.');
		int n_dom = 0;
		int k = 0;
		while (n_dom < nb_domaine) {
			char *token2,*saveptr2,* id,* horodatage,* nom,* code;
			char * copie_buf = malloc(1024*sizeof(char));
			strncpy(copie_buf,buf,strlen(buf)); 
			token2 = strtok_r(copie_buf, "|", &saveptr2);
			id = token2;
			token2 = strtok_r(saveptr2, "|", &saveptr2);
			horodatage = token2;
			token2 = strtok_r(saveptr2, "|", &saveptr2);
			nom = token2;
			token2 = strtok_r(saveptr2, "|", &saveptr2);
			code = token2;
			
			char * saveptr10 = malloc(1024*sizeof(char));
			int b =  nb_occ(saveptr2,'|');
			strcpy(saveptr10,saveptr2);

			for (int g = 0 ; g < 30; g++) 
				memset((char*)tab_buffer[g],'\0',100);
			
			int i = 0;
			//creer un tableau d'index a contacter 
			int tableau_index_contact[b];
			int taille_tabl_index = 0;
			int index_contact = 0;
			for (i = 0; i < b; i++) {
				char *port,*domaine,*addr;
				domaine = strtok_r(saveptr10, ",", &saveptr10);
				addr = strtok_r(saveptr10, ",", &saveptr10);
				port = strtok_r(saveptr10, "|", &saveptr10);
			
				int index_inser = 0;
				
				if ((index_inser = insertion_en_fin(new_c_r,taille_cache-1,domaine,addr,port)) >= 0)  
					tableau_index_contact[index_contact] = index_inser;
				
				index_contact++;
				strncpy(tab_buffer[i],addr,strlen(addr));
			}
		
			int cpt_contact = index_contact - 1;
			int index_c = 0;
			index_contact = tableau_index_contact[index_c];
			
			/*Maintenant que l'on a contacter notre racine on va contacter les serveur de noms et les serveur fin */
				FD_ZERO(&ens);
				int y = 0;
				int c = 0;
				for (y  = 0; y < b;y++) {//on init au cas ou on doit envoyer au trois , nous permettra de faire tourner le tourniquet
					//on check l'adresse avec 
					if (ipv4_or_ipv6(tab_buffer[y]) == 4) 
						CHECK((tableau_sock_fd[y] = socket(AF_INET, SOCK_DGRAM, 0)) != -1);
					else 
						CHECK((tableau_sock_fd[y] = socket(AF_INET6, SOCK_DGRAM, 0)) != -1);
			
					FD_SET(tableau_sock_fd[y],&ens);
				}
				
				
				int goon2 = 1;
				int rm = 0;
				if ((rm = remise_a_zero_domaine(new_c_r,taille_cache,new_c_r[index_contact].domaine)) == 1)
					printf("on peut a nouveau contacter le premier serveur de notre liste qui a pour domaine : %s\n",new_c_r[k].domaine);

				//printf("rm : %d\n",rm);

				int contact_tracing = 0;
				
				while (goon2) {//goon2 pour la boucle faire défiler les k au cas où deja a 1 dans le cache, avant envoie on doit aussi regarder si c'est deja a 1 
							//donc si c'est a 1 on passe au k suivant !! sinon on envoie et on regarde si sa repond.

					int k_1 = c;
					//Faire une recherche dans le cache de la première occurence du domaine que l'on doit contacter (.fr , .com etc )
					//les domaines recherchés serront dans l'ancien recvfrom, on a juste a stocker le nom recu et on fera la recherche en fonction du nom
					// car dans le cache on aura bien trop de choses, et plus il va grandir plus sa sera un peu le bordel à l'intérieur
					// donc il vaut mieux faire une recherche dedans , pour tomber au premier indice de ce domaine et pouvoir faire les sockets et le possible envoie à partir de là

					int V4_V6;
				
					if ((new_c_r[index_contact].boolean == 0) && (contact_tracing == 0)) {
						id_principal = extraction_first_id_chaine(id);
						new_argv2 = change_first_id_chaine(message,id_principal+1);
						new_argv3 = change_horo_chaine(new_argv2,time(NULL));
						if ((V4_V6 = ipv4_or_ipv6(new_c_r[index_contact].addr)) == 4) {
							new.sin_family = AF_INET;
							new.sin_port = htons(atoi(new_c_r[index_contact].port));
							inet_pton(AF_INET, new_c_r[index_contact].addr, &new.sin_addr);
							CHECK((n = sendto(tableau_sock_fd[c], new_argv3, strlen(new_argv3), 0, (struct sockaddr *) &new, longeur)) > 0);
						}
						else {
							new6.sin6_family = AF_INET6;
							new6.sin6_port = htons(atoi(new_c_r[index_contact].port));
							inet_pton(AF_INET6,new_c_r[index_contact].addr, &new6.sin6_addr);
							CHECK((n = sendto(tableau_sock_fd[c], new_argv3, strlen(new_argv3), 0, (struct sockaddr *) &new6, longeur6)) > 0);	
						}
						printf("%s\n",new_argv3);
						free(new_argv2);
						free(new_argv3);
					}

					tmp = ens;
					d = delai;
					CHECK((select(tableau_sock_fd[y-1] + 1  ,&tmp,NULL,NULL,&d) != -1)); 
			
					if (FD_ISSET(tableau_sock_fd[c],&tmp)) {
						size_t r2;
						memset(buf,'\0',taille);
						int indice_in_cache;
						if ((indice_in_cache = addr_port_deja_repondu(new_c_r,taille_cache,new_c_r[index_contact].addr,new_c_r[index_contact].port)) != -1) {
							if(ipv4_or_ipv6(new_c_r[index_contact].addr)==4) 
								CHECK((r2 = recvfrom(tableau_sock_fd[c], buf, taille, 0, (struct sockaddr *) &new, &longeur)) > 0);
							else
								CHECK((r2 = recvfrom(tableau_sock_fd[c], buf, taille, 0, (struct sockaddr *) &new6, &longeur6)) > 0);

							printf("%s\n",buf);
							domaine_addr_port_qui_repond(new_c_r,k,new_c_r[index_contact].domaine,new_c_r[index_contact].addr,new_c_r[index_contact].port);
							contact_tracing = 1;
						}
					}
					else {
						//if (k < b) {
							//c'est que ce n'était pas la dernière donc on envoie à la suivante
							//close(tableau_sock_fd[k]);	
						//}
					}
					k++;
					c++;
					++index_c;
					if (index_c < cpt_contact+1) 
						index_contact = tableau_index_contact[index_c];
						
					if (c >= b)
						goon2 = 0;

					tmp = ens;
				}
				tmp = ens;

			for (int w = 0 ; w < b ; w++)
				close(tableau_sock_fd[w]);

			n_dom++;
			}

			affiche_cache_resolution(new_c_r,20);

		/*Fermeture*/
		if (n_dom >= 2) {
			if (cpt_mot == 0) 
				message = "4|167777777|zombie.eric.fr";
			
			else if (cpt_mot == 1) 
				message = "7|122222222|zombie.eric.fr";
			
			else if (cpt_mot == 2)
				message = "10|19999999|example.eric.fr";

			else if (cpt_mot == 3)
				message = "15|3000000|chris.moodle.fr";
			else if (cpt_mot == 4)
				message = "18|9000000|toto.vs.com";;

			cpt_mot++;
		}


		if(cpt_mot >= 6) 
			goo = 0;
	}
	close(sockfd);
	close(fd);
	return EXIT_SUCCESS;
}
