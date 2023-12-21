#Makefile
#kill -s kill <pid_number> au cas où un des programmes que l'on a lancer ne s'arrete pas 

CC=gcc
CFLAGS=-W -Wall -Werror -Wextra

EXEC = projet

all: $(EXEC)

projet: 
	$(CC) fonctions.c fonctions.h client.c -o client -g 
	$(CC) fonctions.c fonctions.h serveur_racine.c -o serveur -g
	$(CC) fonctions.c fonctions.h serveur_nom.c -o server_nom -g
	$(CC) fonctions.c fonctions.h serveur_fin.c -o server_fin -g
	$(CC) fonctions.c fonctions.h test.c -o test -g
	
clean:
	rm -fr serveur fonctions client server_nom server_fin test
