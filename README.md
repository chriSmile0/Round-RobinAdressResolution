
									Objectifs


//Update du 1/11/2020

						Commandes d'éxecution du projet : 1) make 2) bash execution.sh
										


Pour notre projet nous avons définis 7 règles dans notre documentation


La première -Base N°) sont des règles qui vont évoluer au cours du temps de notre projet

La seconde  -N°) sont les étapes que l'on doit accomplir et qui resteront fixes du moins on l'espère  car si on programme de façon intelligente et méthodique on sais que l'on reviendra dessus mais le moins possible afin de pas changer tout le code qui a était fait depuis 3 semaines

La troisième -N° bis) sont les étapes qui seront en IPV6 donc une fois IVP4 réalisé il va falloir creer l'implémentation pour que cela fonctionne aussi en IPV6 Pour le moment nous allons essayer de faire fonctionner un maximum de choses en IPV4 mais une fois que notre programme fonctionne d'une manière optimal et avec  un certain avancement dans le projet alors on décidera de passer à l'implémentation
de la traduction avec IPV6 

La quatrième -(Validé) Signifie qu'une tâche est bien réalisée et est fonctionnelle 
		       Plus il y'a de tâche sous le topic (Validé) plus notre projet avance

La cinquième -(En cours de traitement) Signifie que cette tâche est en cours de traitement

La sixième   -(Non commencée) Signifie que dans notre code rien n'est fait pour le moment pour 
			      faire en sorte que cette tâche sois réalisée.
			      Plus il y'a de tâche sous le topic (Nom commencée) plus on sais que
			      l'on a du travail

La septième  -(Details des étapes validées) Topic pour présenter les étapes réalisées

Base 1 ) Faire les specs de toutes les fonctions
Base 2 ) Le readme servira de documentation pour l'avancée du projet
									(Validé) 
1)Programme de base entre 1 client et le serveur racine V4
2)Programme de base entre 1 client et le serveur racine V4, ainsi qu'un serveur de nom et un serveur_fin 
3)Envoyer une meme requete a plusieurs serveurs du même niveau (racine ou nom ou fin) et donc modifier l'identifiant si il y en a plusieurs en ajoutant un id-id_secondaire si j'ai plus de 1 serveur a 
contacter




								(En cours de traitement)
1 bis) Programme de base entre 1 client et le serveur racine V6
4)Envoyer une même requête a plusieurs serveurs du même niveau (racineo ou nom ou fin) afin de determiner plus tard lequel nous répond le plus vite et donc on enlevera les autres de notre liste de serveurs de ce type là. Par exemple j'envoie eric.fr a mon serveur racine, je recois donc 2 adresses de .fr et donc j'envoie à 2 serveur different ma requête . Si il n'y en as qu'un seul qui me répond c'est super j'élimine l'autre et je garde celui ci. Mais si les 2 me répondent je doit regarder la différence entre mon horodatage d'envoie sur chacun des serveurs et leur horodatage de réponse et donc quand je fait la soustraction je vois lequel à était le plus rapide et donc je garde le plus rapide et j'enlève le plus lent.
Imaginons que j'envoi une seconde requete mais cette fois si c'est zombie.eric.fr. Je recommence donc comme avant mais cette fois si comme avant mon serveur racine me propose les serveurs .fr a contacter
Je vois bien qu'il ma renvoyer celui que j'avais enlever donc je vais passer par l'autre. Pour le moment on estime que celui ci va fonctionner et que donc je peut continuer et ouvrir un serveur eric.fr
dont l'adresse m'aura était envoyé par le .fr le plus rapide que j'ai obtenu avant. Mais si le .fr que ma proposer la racine ne me repond pas, que faire ? L'idée sera surment de réessayer avec celui que l'on a éliminer avant mais ça on verra par la suite comment on se débrouille avec cela.


									(Nom commencée)

2 bis) Programme de base entre 1 client et le serveur racine V6, ainsi que 3 serveur de nom et un serveur_fin 
3) 

N-1) Tests c , avec tests unitaires, et test et build sur git

								(Details des étapes validées)


Etape Base 1) Creation de fichier.h ainsi que du .c de ce .h  : fonctions.h et fonctions.c .Le fichier.h contient les prototypes et les specifications de chaque fonction.
Le fichier.c contient le code des fonctions

		Nos structures :
		- resolution
		- recv_buf
		- clientmessage

		Les specifications sont donc faites pour les fonctions de base :
		- sock_factory
		- nb_occ
		- rech_domaine
		- prefix
		- creation_mess
		- rech_sousdomaine
		- traiter_mess
		- ipv4_or_ipv6
		- index_char_in_chaine
		- champ_second_id_chaine
		- change_first_id_chaine
		- extraction_second_id_chaine
		- extraction_first_id_chaine


On a creer des fichiers séparés pour que les fichiers qui font les éxécutions comme client et serveur soient les plus petit possible


Etape 1 ) 
Dans mon code le serveur_racine est joignable sur le port 5000 donc de mon coté c'est 5000.Donc quand on met le serveur racine en écoute sur le port 5000 et que le client cherche le numero de port à partir du quel il pourra joindre @.domaine alors le serveur racine lui communiquera le port sur lequel le 'domaine' qui est dans son message se trouve pour qu'ensuite le client interroge sur ce port là et donc cela constitura la phase 2 qui est de faire communiquer le client vers le sous-domaine suivant Là on a reussi l'aller retour numéro 1 avec aller client et retour serveur racine maintenant on doit faire aller client et retour serveur du sous domaine


Etape 2)
Le serveur_racine se met à écouter sur son port 5000. On lance 1 serveur de nom .fr ainsi qu'un serveur de fin eric.fr (comme notre exemple est zombie.eric.fr on sais que eric.fr est notre serveur de fin)
On a voulu mettre l'affichage comme dans le schéma du projet.
Pour le moment cela fonctionne en IPV4 et avec notre adresse locale c'est à dire 127.0.0.1 dans tout nos fichiers (serveur_name_ip) (serveur_name_ip_eric) car on ne peut pas encore tester à la fac 

Notre transition pour passer à l'IPV6 est en train de se mettre en place

Etape 3)
On doit changer les identifiants des messages que l'on envoie avec le client, donc comme dans le schéma de l'exemple, j'envoie l'id 1 , le serveur racine me renvoie 1 puis je renvoie 2 au serveur de nom 
puis 3 au serveur fin 
Et on a imaginer que si on veut commniquer avec plusieurs serveur_racine serveur_nom serveurfin  on met un id donc  id principal-id secondaire , de tel sorte à ce quand on veut contacter plusieurs type de serveurs et donc par exemple les serveurs racine cela fera 1-1 au serveur 1 , 1-2 au serveur 2 et ainsi de suite, et idem pour les serveur_nom et les serveurs fin

Etape4) 
Envoyer un horodatage coherent et faire la soustraction à chaque fois , on va creer les fonctions pour le gerer mais sa pourrait être de la même acabit que l'étape 3 , c'est en cours ...



REPARTITION DES PORTS DANS LE BASH:

				IPV4				IPV6

Serveur racine :

	port = 		[5000 - 5010[			[5010 - 5020[

Serveur nom : 			[5020 - 5060[			[5060 - 6000[

	fr = 			[5020 - 5030[			[5060 - 5070[
	com = 			[5030 - 5040[			[5070 - 5080[
	
Serveur_fin :			[6000 - 6050[			[6050 - 7000[

	vs = 			[6000 - 6010[			[6050 - 6060[
	eric = 		[6010 - 6020[			[6060 - 6070[
	moodle = 		[6020 - 6030[			[6070 - 6080[
	example = 		[6030 - 6040[			[6080 - 6090[
	
Machine :			7000 +









