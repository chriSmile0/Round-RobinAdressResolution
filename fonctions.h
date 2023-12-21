#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <stdio.h> // For fprintf, perror
#include <stdlib.h> // For exit
#include <time.h> //for time


typedef struct resolution {
	char * domaine;
	char * addr;
	char * port;
} resolution;


typedef struct cache_resolution {
	char * domaine;
	char * addr;
	char * port;
	int boolean;

} cache_resolution;




typedef struct recv_buf {
	char * domaine;
	char * addr;
	char * port;
} recv_buf;

typedef struct{
	char * id;
	char * horodatage;
	char * nom;
} clientmessage;

/*typedef struct {
	char * first_horo;
	char * second_horo;
	int difference;
} tab_horo_with_diff;*/


 /**
 * @brief Fonction qui va creer une socket et lié cette socket
 *        a une structure sockaddr_in 
 * 
 * @param[:port] Une chaine de caractère qui sera utiliser pour transmettre
 *               l'argument du programme serveur_racine pour attribuer le
 *               numero de port à cette structure 
 * 
 * @return On retourne un entier qui est l'identifiant de cette socket
 * 
*/

int sock_factory(char *port);

/**
 * @brief Fonctione qui va donner le nombre d'occurences 
 * 		  du caractere c passé en paramètre
 * 
 * @param[:chaine] La chaine de caractère dans laquelle on cherche le nombre 
 * 				   d'occurences du paramètre c
 * @param[:c] Le caractere que l'on recherche 
 * 
 * @return nbc qui est le nombre d'occurences de c dans la chaine 
 * 
*/
int nb_occ(char *chaine,char c);

/**
 * @brief Fonction qui recherche le dernier domaine du buffer passer 
 * 		  en paramètre c'est à dire que l'on recherche le domaine 
 * 		  qui va contenir tout les autres sous domaines et ainsi de suite
 * 		  par exemple un .fr ou un .com qui sont les derniers domaines 
 * 		  d'une recherche sur le web
 * 
 * @param[:buffer] Le buffer dans lequel on va extraire le dernier domaine
 * 
 * @param[:occ] Le nombre d'occurence du caractere '.' après lequel on trouvera
 * 				le dernier domaine du buffer
 * 
 * @return Retourne le dernier domaine de la chaine sous forme d'une chaine
 * 
*/

char * rech_domaine(char * buffer, int occ);

/**
 * @brief Fonction qui cree une chaine de caractere correspondant aux prefix du message à envoyer
 * 
 * @param[:domain] le domain dans lequel se trouve l'adresse de la requete cliente
 * 
 * @return le prefix du message en chaine de caractere
 * 
*/

char * prefix(char * domain);

/**
 * @brief Fonction qui cree le message à envoyer aux clients 
 * 
 * @param[:file] l'adresse du flux de fichier dans lequel se trouve les ips correspondants aux serveurs de domaines
 * 
 * @param[:domain] le domain dans lequel se trouve l'adresse de la requete cliente 
 * 
 * @return le message à envoyer aux clients en chaine de caratère
 * 
*/

char * creation_mess(FILE * file , char *  domain, char * identifiant, char * nom, char * horo);

/**
 * @brief Fonction qui recherche le sous domaine du buffer passer 
 * 		  en paramètre c'est à dire que l'on recherche le domaine 
 * 		  qui va contenir tout les autres sous domaines et ainsi de suite
 * 		  par exemple un .fr ou un .com qui sont les derniers domaines 
 * 		  d'une recherche sur le web
 * 
 * @param[:buffer] Le buffer dans lequel on va extraire le sous domaine
 * 
 * @param[:occ] Le nombre d'occurence du caractere '.' après lequel on trouvera
 * 				le sous domaine du buffer
 * 
 * @return Retourne le sous domaine de la chaine sous forme d'une chaine
 * 
*/
char * rech_sousdomaine(char * buffer, int occ);

/**
 * @brief Fonction qui extrait du buffer passer en parametre (c.f requête)
 *		  le nom , l'id et l'horodatage de la requête
 * 
 * @param[:buffer] Le buffer examiner pour extraction des donnees
 * 
 * @return Retourne une structure clientmessage qui contient le nom
 *         l'id et l'horodatage de la requête
 * 
*/

clientmessage traiter_mess(char * buffer);

/**
 * @brief Fonction qui cherche si la chaine entrer en parametre
 * 		  qui represente une adresse est une adresse IPV6
 *        ou une adresse IPV6
 * 
 * @param[:chaine] La chaine a examiner
 * 
 * @return Retourne un entier qui es sois 4 pour une IPV4
 *         et donc 6 pour une IPV6
 * 
*/

int ipv4_or_ipv6(char * chaine);

/**
 * @brief Fonction qui cherche si un caractere precis est dans une chaine
 * 
 * @param[:chaine] La chaine a examiner
 * 
 * @param[:c] Le caractere a chercher 
 *
 * @return Retoune l'indice du caractere dans la chaine ou -1 en cas d'echec
 * 
*/

int index_char_in_chaine(char *chaine,char c);

/**
 * @brief Fonction qui change le second indice d'une chaine
 *        cette chaine represente ce que l'on va envoyer au serveur
 *        donc on modifie l'identifiant secondaire pour pouvoir identifier
 *        pour identifier les differentes instructions venant du client 
 *        vers des serveurs de noms qui sont au meme niveau
 * 
 * @param[:chaine] La chaine où l'on va changer le second identifiant 
 *
 * @param[:k] Le nombre par lequel on remplace l'ancien second identifiant 
 * 
 * @return Retourne une chaine de caractere qui est une copie de celle entree 
 *         en parametre avec l'identifiant secondaire qui est remplacer par k
 * 
*/

char * change_second_id_chaine(char *chaine, int k);

/**
 * @brief Fonction qui change le premier indice d'une chaine
 *        cette chaine represente ce que l'on va envoyer au serveur
 *        donc on modifie l'identifiant principal pour pouvoir identifier
 *        de quel niveau il s'agit. Par exemple j'envoie l'instruction
 *        avec l'id principal 1 au serveur racine, il me repond avec le meme
 *        et donc j'envoie un autre identifiant principal a un serveur de nom
 *        que le serveur racine m'a recommander et donc on enverra la suite 
 *        avec l'identifiant principal 2 par exemple
 * 
 * @param[:chaine] La chaine où l'on va changer le second identifiant 
 *
 * @param[:k] Le nombre par lequel on remplace l'ancien second identifiant 
 * 
 * @return Retourne une chaine de caractere qui est une copie de celle entree 
 *         en parametre avec l'identifiant secondaire qui est remplacer par k
 * 
*/

char * change_first_id_chaine(char *chaine,int id);

/**
 * @brief Fonction qui extrait le second id d'une chaine a condition
 *		  que le second id sois present
 *       
 * 
 * @param[:chaine] La chaine où l'on va changer le second identifiant 
 *
 * @return Retourne un entier qui correspond à l'id chercher sinon -1 si pas 
 *         de second id
 * 
*/

int extraction_second_id_chaine(char * chaine);

/**
 * @brief Fonction qui extrait le premier id d'une chaine qui est la requête
 *       
 * @param[:chaine] La chaine où l'on va changer le premier identifiant 
 *
 * @return Retourne un entier qui correspond à l'id cherche
 *
*/

int extraction_first_id_chaine(char * chaine);

char * change_horo_chaine(char * chaine,int horodatage);

int extract_horo_chaine_int(char *chaine);

char * extract_horo_chaine_char(char * chaine);


/*------------------------------------------------Nouvelle phase du projet ---------------------------------------------*/
/**
* @brief Fonction qui va initialiser les chaines de caracteres a un espace vide
*        de taille 1024 et les booleans a 0 
*
* @param[:*c_r] le cache 
* 
* @param[:taille] la taille du cache 
*
* @return pas de retour mais un cache initialiser
*
*/

void init_cache_resolution(cache_resolution *c_r, int taille);

/**
* @brief Fonction qui va afficher le cache , donc le domaine, l'adresse,
*        le port et le boolean
*
* @param[:*c_r] le cache 
* 
* @param[:taille] la taille du cache 
*
* @return pas de retour 
*
*/

void affiche_cache_resolution(cache_resolution *c_r, int taille);

/**
* @brief Fonction qui insere le domaine, l'adresse , le port et qui met
*        le boolean a 0 dans le cache
*
* @param[:*c_r] le cache 
*
* @param[:index] l'index où inserer dans le cache 
*
* @param[:domaine] le domaine a inserer
*
* @param[:addr] l'adresse a inserer
*
* @param[:port] le port a inserer
*
* @return retourne -1 si le cache n'est pas lisible, 1 si insertion effectuee,
*		  0 sinon 
*
*/

int present_in_cache(cache_resolution *c_r,int taille, char * domaine,char * addr, char *port);

int inserer_domaine_addr_port_in_cache_a_contacter(cache_resolution *c_r,int index ,int taille ,char * domaine,char * addr,char * port);

int insertion_en_fin(cache_resolution *c_r,int taille,char *domaine,char *addr,char *port);

/**
* @brief Fonction qui renvoie l'index si le domaine, l'adresse et le port et
*        si le boolean est a 0 et qui met le boolean a 1 pour signifier
*        que l'adresse qui correspond a ce domaine a bien repondu sur ce port
*
* @param[:*c_r] le cache 
*
* @param[:index] l'index où inserer dans le cache 
*
* @param[:domaine] le domaine a inserer
*
* @param[:addr] l'adresse a inserer
*
* @param[:port] le port a inserer
*
* @return retourne -1 si le cache n'est pas lisible, index si ce dernier a
*         a repondu 0 sinon
*
*/

int domaine_addr_port_qui_repond(cache_resolution *c_r,int index ,char * domaine,char * addr, char * port);

/**
* @brief Fonction qui va donner l'indice de la premiere case qui corresponde 
*        a l'adresse et au port rechercher et en verifiant que le boolean est a 1
*
* @param[:*c_r] le cache 
* 
* @param[:taille] la taille du cache 
* 
* @param[:addr] l'adresse que l'on cherche	(manque peut etre domaine)
*
* @param[:port] le port que l'on cherche
*
* @return retourne l'indice du premier duo port-adresse qui a repondu 
*, 		  -1 si le cache n'est  pas lisible, et 0 sinon
*
*/

int addr_port_deja_repondu(cache_resolution *c_r,int taille, char * addr, char * port);


/**
* @brief Fonction qui va mettre a 0 tout les cache qui ont un domaine
*		 entrer en parametre a condition qu'ils ont tous étaient mi a 1 avant 
*
* @param[:*c_r] le cache que l'on va faire tourner en interne
* 
* @param[:taille] la taille du cache 
*
* @param[:domaine] le domaine a reinitialiser
*
* @return pas de retour mais un cache modifier
*
*/

int remise_a_zero_domaine(cache_resolution *c_r,int taille, char * domaine);



/**
* @brief Fonction qui va proceder au tourniquet des differents domaines
*
* @param[:*c_r] le cache que l'on va faire tourner en interne
* 
* @param[:taille] la taille du cache 
*
* @return on retourne -1 si cache non lisible, 0 si pas de remise a zero
*         possible , et 1 si on a reussi a tout remettre a 0
*
*/

void round_robin(cache_resolution *c_r,int taille);




#endif /* __Fonctions_H__ */
