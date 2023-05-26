
							Documentation Utilisateur

MODE D'EMPLOI:

Un makefile est à votre disposition pour compiler tous les fichiers avec différents options avec la commande make.

Pour executer le client : ./client liste_serveur_racine listes_de_nom à resoudre 
pour executer le serveur racine:  ./serveur liste_adresse_connue port_ipv4 port_ipv6
Pour executer le serveur de noms et les serveurs finaux: ./server_nom serveur liste_adresse_connue port_ipv4 port_ipv6 sleep

Pour tester notre implémentation, un fichier bash est à votre disposition avec la commande : bash execution.sh dans lequel l'attribution des ports a été faite de maniere suivante:

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













# Round-RobinAdressResolution
