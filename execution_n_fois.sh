#!/bin/bash


executer_n_fois_prog ()  #param 1 : le programme , param 2 : n fois
{
    n=$2
    n=$(($n - 1))
    for i in `seq 0 $n`
    do
        bash $1
        #echo "i "$i
    done


}

executer_n_fois_prog "execution.sh" 1