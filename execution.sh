#!bin/bash

#bash a executer pour voir la beauté



executer_n_fois_commande ()  #param 1 : le programme , param 2 : n fois
{
    n=$2
    n=$(($n - 1))
    for i in `seq 0 $n`
    do
        $1
    done


}

#executer_n_fois_commande "echo hello" 1 # a utiliser pour faire tourner n fois des make test



executer_prog_client ()  
{
    PROG=./"$1"
   
    $PROG

}

executer_prog_racine () 
{
    PROG=./"$1"
    fichier=$2 # le fichier
    port4=$3 # le port
    port6=$4 # le port
    $PROG $fichier $port4 $port6
}

executer_prog_serveur_nom ()
{
    PROG=./"$1"
    fichier=$2 # le nom de fichier
    port4=$3 # le port
    port6=$4 # le port 
    $PROG $fichier $port4 $port6
}

executer_prog_serveur_fin ()
{
    PROG=./"$1"
    fichier=$2 # le nom de fichier
    port4=$3 # le port
    port6=$4 # le port  
    $PROG $fichier $port4 $port6
}



executer_prog_racine serveur "serveurip" 5000 5010 &   # le esperluette permet de passer le prog en arriere plan


executer_prog_serveur_nom server_nom "serveur_name_ip" 5020 5060 & 

executer_prog_serveur_nom server_nom "serveur_name_ip" 5021 5061 &

executer_prog_serveur_nom server_nom "serveur_name_ip" 5022 5062 &

executer_prog_serveur_nom server_nom "serveur_name_ip_com" 5030 5070 &

executer_prog_serveur_fin server_fin "serveur_name_ip_vs" 6000 6050 & 

executer_prog_serveur_fin server_fin "serveur_name_ip_eric" 6010 6060 &

executer_prog_serveur_fin server_fin "serveur_name_ip_eric" 6011 6061 &

executer_prog_serveur_fin server_fin "serveur_name_ip_moodle" 6020 6070 &


#executer_prog_serveur_fin server_fin "serveur_name_ip_moodle" 5012 &

executer_prog_client client # a utiliser pour tester les prog 'classique' # on est sur le cas simple pour le moment

#executer_prog_serveur_nom server_nom "serveur_name_ip" 5800 &  # le esperluette permet de passer le prog en arriere plan

#executer_prog_serveur_fin server_fin "serveur_name_ip_eric" 5006 &  # le esperluette permet de passer le prog en arriere plan


IFS=' 
 '

pid_serveur=$(ps -C "serveur" -o pid=) 
pid_serveur_nom=( $(ps -C "server_nom" -o pid=) ) 
pid_serveur_fin=( $(ps -C "server_fin" -o pid=) ) 

kill -s kill  $pid_serveur

kill -s kill  ${pid_serveur_nom[0]}
kill -s kill  ${pid_serveur_nom[1]}
kill -s kill  ${pid_serveur_nom[2]}
kill -s kill  ${pid_serveur_nom[3]}


kill -s kill  ${pid_serveur_fin[0]}
kill -s kill  ${pid_serveur_fin[1]}
kill -s kill  ${pid_serveur_fin[2]}
kill -s kill  ${pid_serveur_fin[3]}












