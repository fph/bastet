#!/bin/bash

#This script iterates lang, this array contains the languages ​​in which the game was translated-
#In each iteration, generates the .mo binary file and copy this into the folder /usr/share/locale/xx_XX/LC_MESSAGES /, where xx_XX is the language in which it was translated.
#Finally the .mo binary file is remove of local repository. 

lang=('es_AR' 'pt_BR' 'it_IT')

for i in "${lang[@]}" 
do
	sudo msgfmt -c -v -o ./po/$i/main.mo ./po/$i/main.po
	sudo mkdir -p /usr/share/locale/$i
	sudo mkdir -p /usr/share/locale/$i/LC_MESSAGES
	sudo cp ./po/$i/main.mo /usr/share/locale/$i/LC_MESSAGES
	rm -rf ./po/$i/main.mo
done

