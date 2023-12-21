#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "check.h"
#include "fonctions.h"

int sock_factory(char *port)
{
    int sockfd;
    struct sockaddr_in new;
    CHECK(sockfd = socket(AF_INET, SOCK_DGRAM, 0));

	memset((char *)&new,0,sizeof(new));
	new.sin_family = AF_INET;
	new.sin_port = htons(atoi(port));
	new.sin_addr.s_addr = htonl(INADDR_ANY);
    
    CHECK(bind(sockfd, (struct sockaddr *)&new,
	       sizeof(struct sockaddr_in)) != -1);

    return sockfd;
}


int nb_occ(char *chaine,char c) 
{
    int i,size,nbc = 0;
    size = strlen(chaine);    
     
    for (i = 0 ; i < size ; i++) 
        if (chaine[i] == c) 
            nbc ++;  
        
    return nbc;             
}


char * rech_domaine(char * buffer, int occ){
	char *token,*saveptr;
	
	token = strtok_r(buffer, ".", &saveptr);
	
	for (int i = 0; i < occ; i++) 
		token = strtok_r(saveptr, ".", &saveptr);
    
	
	return token;
}

char * prefix(char * domain)
{
	char * chaine = malloc (1024 * sizeof(char));
	snprintf(chaine, strlen(chaine)+1, "%c", '.'); 
	snprintf(chaine, strlen(chaine)+strlen(domain), "%s", domain);
	snprintf(chaine, strlen(chaine)+1, "%c", ','); 
	//strcat(strncat(strcat(chaine ,"."),domain,strlen(domain)),", ");
	
	return chaine;
}

char * creation_mess(FILE * file , char *  domain, char * identifiant, char * nom, char * horo)
{
	/*char horo[20];
	time_t timestamp = time( NULL );
	snprintf(horo, 20, "%ld", timestamp);*/
	char * ligne = (char *) malloc(128);
	char * ip_port = (char *) malloc(128);
	char *token;
	char *saveptr;
	char *ip_port_serv =  malloc (1024 * sizeof(char)); 
	char * message =  malloc (1024 * sizeof(char));
	memset(message,'\0',1024);
	memset(ip_port_serv,'\0',1024);
	char * id = identifiant;
	char * pre = prefix(domain);
	fseek(file,0,SEEK_SET);
	int n =0;
	while (fgets(ligne,128,file) != NULL) {
		char * temp = strstr(ligne,domain); 
	
		if (temp != NULL) {
			memset(ip_port_serv,'\0',1024);
			n++;
			if (n == 1)
				strcat(strcat(strcat(strcat(strcat(strcat(strcat(strncat(message,id,strlen(id))," | "),horo)," | "),nom)," | ")," 0 ")," | ");
			strcpy(ip_port, ligne);
	
			token = strtok_r(ip_port, "|", &saveptr);
			token = strtok_r(saveptr, "|", &saveptr);
			char * ip = token;
			token = strtok_r(saveptr, "|", &saveptr);
			char * port = token;
			char * chaine = strchr(port,'\n');
			if(chaine!=NULL)
				*chaine = '\0';
			strcat(strcat(strcat(strcat(strcat(ip_port_serv,pre),ip),", "),port)," | ");
			strcat(message,ip_port_serv);
		}
	}
	if (n == 0) {
		strcat(strcat(strcat(strcat(strcat(strcat(strcat(strcat(message,id)," | "),horo)," | "),nom),"|"),"-1")," | ");
		strcat(ip_port_serv,pre);
		strcat(message,ip_port_serv);
	}
	
	free(ligne);
	free(ip_port);
	free(ip_port_serv);
	return message;
}





char * rech_sousdomaine(char * buffer, int occ) 
{
	char *token;
	
	for (int i = 0; i < occ-1; i++) 
		token = strtok_r(buffer, ".", &buffer);
   
	return buffer ;
}

clientmessage traiter_mess(char * buffer)
{
	clientmessage c1;
	char *token,*saveptr;
	
	token = strtok_r(buffer, "|", &saveptr);
	if (token == NULL){
		c1.id = "-1";
		printf("ERREUR ERREUR ERREUR ERREUR");
		return c1;
	}
	c1.id = token;
	token = strtok_r(saveptr, "|", &saveptr);
	if (token == NULL || saveptr == NULL){
		c1.id = "-1";
		printf("ERREUR ERREUR ERREUR ERREUR");
		return c1;
	}
	c1.horodatage = token;
	c1.nom = saveptr;
	
	return c1;
}

int ipv4_or_ipv6(char *chaine) 
{
	int i = 0;
	while (((chaine[i] != '.') && (chaine[i] != ':')) && (chaine[i] != '\0' )) 
		i++;
	
	if (chaine[i] == '.')
		return 4;
	else
		return 6;

}

int index_char_in_chaine(char *chaine,char c)
{
	int i = 0;
	while ((chaine[i] != c) && (chaine[i]!= '\0'))
		i++;

	if (i == strlen(chaine))
		return -1;
	else	
		return i;

}

char * change_second_id_chaine(char *chaine, int k)
{
	char * new_chaine = malloc((strlen(chaine)+1)*sizeof(char));
	char *saveptr;
	snprintf(new_chaine,strlen(chaine)+1,"%s",chaine);//on remplace le strncpy par un truc plus safe comme snprintf
	char *new = strtok_r(new_chaine,"|",&saveptr);

	char * ligne = NULL;
	ligne = (char *)malloc(1024*sizeof(char));
	memset(ligne,'\0',1024);

	int index_tiret = index_char_in_chaine(new,'-');
	if (index_tiret == -1) {
		snprintf(ligne,strlen(new)+4,"%s",new);
		int b = strlen(ligne);
		ligne[b] = '-';
		if (k < 10) {
			ligne[b+1] = (k)+'0';
			ligne[b+2] = '|';
		}
		else {
			ligne[b+1] = ((k)/10)+'0';
			ligne[b+2] = ((k)%10)+'0';
			ligne[b+3] = '|';
		}
		strncat(ligne,saveptr,(strlen(saveptr)));
	}
	else {
		char * saveptr2;
		int x = index_char_in_chaine(new_chaine,'-');
		char c = new_chaine[x];
		char * new_c = strtok_r(new_chaine,"-",&saveptr2);
		snprintf(ligne,strlen(new_c)+4,"%s",new_c);
		sprintf(ligne+strlen(ligne),"%c",c);
		int b = strlen(ligne);
		if (k < 10) {
			sprintf(ligne+b,"%c",((k)+'0'));
			sprintf(ligne+b+1,"%c",'|');
		}
		else {
			sprintf(ligne+b,"%c",((k/10)+'0'));
			sprintf(ligne+b+1,"%c",((k%10)+'0'));
			sprintf(ligne+b+2,"%c",'|');
		}
		strcat(ligne,saveptr);
	}		
	free(new_chaine);
	return ligne;
}

char * change_first_id_chaine(char *chaine,int id) 
{
	char * new_chaine = NULL; 
	new_chaine = (char *)malloc(1024*sizeof(char));
	if (new_chaine == NULL)
		printf("eror \n");
	memset(new_chaine,'\0',1024);
	char *saveptr;
	
	strncpy(new_chaine,chaine,strlen(chaine)+1);
	char * new = strtok_r(new_chaine,"|",&saveptr);
	
	char * ligne = malloc(1024*sizeof(char));
	memset(ligne,'\0',1024);
	int index_tiret = index_char_in_chaine(new_chaine,'-');
	if (index_tiret == -1) {
		if (id < 10) {
			ligne[0] = (id)+'0';
			ligne[1] = '|';	
		}
		else {
			ligne[0] = ((id)/10)+'0';
			ligne[1] = ((id)%10)+'0';
			ligne[2] = '|';
		}
	}
	else {
		char *saveptr2;
		strtok_r(new_chaine,"-",&saveptr2);
		if (id < 10) {
			ligne[0] = (id)+'0';
			ligne[1] = '-';
		}
		else {
			ligne[0] = ((id)/10)+'0';
			ligne[1] = ((id)%10)+'0';
			ligne[2] = '-';
		}
		strcat(ligne,saveptr2);
		strcat(ligne,"|");
	}

	strcat(ligne,saveptr);
	free(new_chaine);
	return ligne;
	 
}

int extraction_first_id_chaine(char * chaine)
{
	int id_principal = 0;
	char * copie_chaine = malloc(strlen(chaine)+1*sizeof(char));
	snprintf(copie_chaine,strlen(chaine)+1,"%s",chaine);
	int x = index_char_in_chaine(copie_chaine,'-');
	if (x == -1) {
		id_principal = atoi(copie_chaine);
	}
	else {
		char * saveptr;
		id_principal = atoi(strtok_r(copie_chaine,"-",&saveptr));
	}
	free(copie_chaine);
	return id_principal;
}

int extraction_second_id_chaine(char * chaine)
{
	int id_secondaire = 0;
	char * copie_chaine = malloc(strlen(chaine)+1*sizeof(char));
	snprintf(copie_chaine,strlen(chaine)+1,"%s",chaine);
	int x = index_char_in_chaine(copie_chaine,'-');
	if (x == -1) {
		id_secondaire = -1;
	}
	else {
		char * saveptr;
		char * new = strtok_r(copie_chaine,"-",&saveptr);
		id_secondaire = atoi(saveptr);
	}
	free(copie_chaine);
	return id_secondaire;
}

char * change_horo_chaine(char * chaine, int horodatage)//on estime que l'on prend la chaine entière en parametre
{
	char * copie_chaine = malloc(1024*sizeof(char));
	snprintf(copie_chaine,strlen(chaine)+1,"%s",chaine);
	char * saveptr,*saveptr2;
	char * id = strtok_r(copie_chaine,"|",&saveptr);

	char * horo = strtok_r(saveptr,"|",&saveptr2);

	char new_horo[20];
	snprintf(new_horo,20,"%d",horodatage);
	char * ligne = malloc(1024*sizeof(char));
	strcat(strcat(strcat(strcat(strcat(ligne,id),"|"),new_horo),"|"),saveptr2);
	free(copie_chaine);
	return ligne;
}

int extract_horo_chaine_int(char *chaine)
{	
	char * copie_chaine = malloc(1024*sizeof(char));
	snprintf(copie_chaine,strlen(chaine)+1,"%s",chaine);
	char * saveptr,*saveptr2;
	char * id = strtok_r(copie_chaine,"|",&saveptr);
	char * horo = strtok_r(saveptr,"|",&saveptr2);
	int d = atoi(horo);
	free(copie_chaine);
	return d;
}

char * extract_horo_chaine_char(char * chaine)
{
	char * copie_chaine = malloc(1024*sizeof(char));
	snprintf(copie_chaine,strlen(chaine)+1,"%s",chaine);
	char * saveptr,*saveptr2;
	char * id = strtok_r(copie_chaine,"|",&saveptr);
	char * horo = strtok_r(saveptr,"|",&saveptr2);
	char *new_horo = malloc(20*sizeof(char));
	snprintf(new_horo,20,"%s",horo);
	free(copie_chaine);
	return new_horo;
}


void init_cache_resolution(cache_resolution *c_r,int taille) //good
{
	if (c_r == NULL) 
		exit(EXIT_FAILURE);
	

	int i = 0;
	while (i < taille) {
		c_r[i].domaine = malloc(100*sizeof(char));
		memset(c_r[i].domaine,'\0',100);
		c_r[i].addr = malloc(15*sizeof(char));
		memset(c_r[i].addr,'\0',15);
		c_r[i].port = malloc(5*sizeof(char));
		memset(c_r[i].port,'\0',5);
		c_r[i].boolean = 0;
		i++;
	}
}

void affiche_cache_resolution(cache_resolution *c_r,int taille) //good
{
	if (c_r == NULL) 
		exit(EXIT_FAILURE);
	

	int i = 0;
	while (i < taille) {
		printf("%s\n",c_r[i].domaine);
		printf("%s\n",c_r[i].addr);
		printf("%s\n",c_r[i].port);
		printf("%d\n",c_r[i].boolean);
		i++;
	}

}

int inserer_domaine_addr_port_in_cache_a_contacter(cache_resolution *c_r,int taille,int index,char * domaine,char * addr,char * port) //EDIt opti insertion
{
	if (c_r == NULL)
		return -1;

	int i = 0;
	printf("index : %d\n",index);
	while  ((((strncmp(c_r[i].domaine,domaine,strlen(domaine))) != 0 ) ||
		(strncmp(c_r[i].addr,addr,strlen(addr)) != 0)  ||
		(strncmp(c_r[i].port,port,strlen(port)) != 0)) &&
		(i < taille)) {
		printf("domaine : %s , domaine_cr %s\n",domaine,c_r[i].domaine);
		i++;
	}

	
	if (i == taille) {//donc si c'est pas encore dedans on insere
		snprintf(c_r[index].domaine,strlen(domaine)+1,"%s",domaine); 
		snprintf(c_r[index].addr,strlen(addr)+1,"%s",addr); 
		snprintf(c_r[index].port,strlen(port)+1,"%s",port); 
		return -2;//insertion reussi
	}
	else {//recherche indice de la premire occurence de cette recherche , //on ne fait rien mais on retourne l'indice
		return i;
	}
	
	return -3;
}

int present_in_cache(cache_resolution *c_r,int taille, char * domaine,char * addr, char *port)
{
	if ( c_r == NULL)
		return -2;

	int i = 0;
	while (((strncmp(c_r[i].domaine,domaine,strlen(domaine))) != 0 ) | 
		(strncmp(c_r[i].addr,addr,strlen(addr)) != 0)  |
		(strncmp(c_r[i].port,port,strlen(port)) != 0) &&
		(i < taille)) 
			i++;
		
	if ( i == taille)
		return -1;//pas present
	else 
		return i;//indice de où il est present

	return -3;

}


int insertion_en_fin(cache_resolution *c_r,int taille,char *domaine,char *addr,char *port) 
{
	if ( c_r == NULL)
		return -1;

	int i = 0;

	int insertion_i = 0;
	if ((insertion_i  = present_in_cache(c_r,taille,domaine,addr,port))==-1) {
		while ((i < taille) && (strlen(c_r[i].domaine) != 0 )) 
			i++;

		snprintf(c_r[i].domaine,strlen(domaine)+1,"%s",domaine); 
		snprintf(c_r[i].addr,strlen(addr)+1,"%s",addr); 
		snprintf(c_r[i].port,strlen(port)+1,"%s",port);
		return i;
	}

	return insertion_i;
}

int domaine_addr_port_qui_repond(cache_resolution *c_r,int index, char * domaine,char * addr, char * port) //good
{
	if(c_r == NULL)
		return -1;

	if (((strncmp(c_r[index].domaine,domaine,strlen(domaine))) == 0 ) && 
		(strncmp(c_r[index].addr,addr,strlen(addr)) == 0)  && 
		(strncmp(c_r[index].port,port,strlen(port)) == 0) &&
		(c_r[index].boolean == 0)) {//un peu de secu sa ne fait pas de mal 

		c_r[index].boolean = 1;
		return 1;
	}

	return 0;
}


int addr_port_deja_repondu(cache_resolution *c_r,int taille, char * addr, char * port) //retourne l'indice du premier port qui a repondu sinon -1 si pas de reponse
{
	if (c_r == NULL)
		return -1;

	int i = 0;
	while (((strncmp(c_r[i].addr,addr,strlen(addr)) != 0)  && 
		((strncmp(c_r[i].port,port,strlen(port)) != 0)) &&
		(c_r[i].boolean != 1))   &&
		(i < taille)) 
		i++;

	if (i < taille) 
		return i;

	return -1;

}

int remise_a_zero_domaine(cache_resolution *c_r,int taille, char * domaine)
{	
	if (c_r == NULL)
		return -1;

	int i = 0;
	int cpt_actif = 0;
	int cpt_inactif = 0;
	int cpt_correspondance = 0;
	
	while ( i < taille) {//on cherche d'abord qu'ils sont tous a 1
		if ((strncmp(c_r[i].domaine,domaine,strlen(domaine))==0) && (c_r[i].boolean == 1)) 
			cpt_actif++;

		else if((strncmp(c_r[i].domaine,domaine,strlen(domaine))==0) && (c_r[i].boolean == 0)) 
			cpt_inactif++;
			
		if (strncmp(c_r[i].domaine,domaine,strlen(domaine))==0) 
			cpt_correspondance++;
	
		i++;
	}
	i = 0;
	if(cpt_actif == cpt_correspondance) {//tout remettre a 0
		while ( i < taille) { //on cherche d'abord qu'ils sont tous a 1
			if (strncmp(c_r[i].domaine,domaine,strlen(domaine))==0)
				c_r[i].boolean = 0;
			i++;
		}
		return 1;
	}



	return 0;
}



void round_robin(cache_resolution *c_r,int taille) 
{
	if (c_r == NULL)
		exit(EXIT_FAILURE);

	/*a completer*/

}
