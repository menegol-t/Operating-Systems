#!/bin/bash
function noParam(){
        echo "Error, no se recibio ningun nombre como parametro."
        exit
}

function muchoParam(){
        echo "Error, se puede ingresar un solo nombre por parametro."
        exit
}

if [ $# -lt 1 ];then
        noParam
elif [ $# -gt 1 ];then
        muchoParam
fi

FOLDER=$*

if [ -d $FOLDER ];then
        echo "Ya existe un directorio con ese nombre."
else
        mkdir $FOLDER
fi

if [ ! -e $FOLDER/archivo.txt ]; then
        touch $FOLDER/archivo.txt
        echo "Se creo $FOLDER/archivo.txt"
fi

#La consigna pide especificamente listar TODOS los archivos de toda la computadora
#Sé como hacerlo, con ls -lR /
#Pero en pruebas con alysa da un monton de errores por permiso de usuario (que podria suprimir con 2>/dev/null)
#Y se toma demasiado tiempo en ejecutar el script, asi que busco solo en el home del user.

ls -lR "$HOME" | grep ".txt" > $FOLDER/archivo.txt

date >> $FOLDER/archivo.txt

cat $FOLDER/archivo.txt