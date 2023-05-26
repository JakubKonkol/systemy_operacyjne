#!/bin/bash
# JAKUB KONKOL S24406
# Skrypt pisalem na systemie macOS uzywajac komendy `bash zadanie.sh` dlatego skladnia komendy stat moze sie troche roznic i nie dzialac na systemie ubuntu
# Umieszce tu odpowiedniki komend  `stat` zeby dzialaly na systemie ubuntu
# funkcja getDirectoryRights() (linia 50) stat -c '%a' "$1"
# funkcja getOwner() (linia 59) stat -c '%U' "$1"
# funkcja getLastModified() (linia 63) stat -c '%y' -t '%d.%m.%Y' "$1"
#
#
function getPath(){
   if [ -d "$1" ]; then
        echo "OK";
    else
        echo "This path isn't a directory or doesn't exist"
        exit 0
    fi
}
function getRightsOwner(){
    # echo "$1"
    if [ "$1" == 0 ]; then
        printf "Owner:\n"
    elif [ "$1" == 1 ]; then
        printf "Group:\n"
    elif [ "$1" == 2 ]; then
        printf "Others:\n"
    else 
        printf "eeeeee"
    fi
}
function getRightDescription(){
    if [ "$znak" == 7 ]; then
            printf "\tfull access\n"
        elif [ "$znak" == 6 ]; then
            printf "\tread, write\n"
        elif [ "$znak" == 5 ]; then
            printf "\tread, execute\n"
        elif [ "$znak" == 4 ]; then
            printf "\tread\n"
        elif [ "$znak" == 3 ]; then
            printf "\twrite, execute\n"
        elif [ "$znak" == 2 ]; then
            printf "\twrite\n"
        elif [ "$znak" == 1 ]; then
            printf "\texecute\n"
        elif [ "$znak" == 0 ]; then
            printf "\tno access\n"
        fi
}
function getDirectoryRights() {
    permissions=$(stat -f '%A' "$1")
    for (( i=0; i<${#permissions}; i++ )); do
        znak=${permissions:i:1}
        getRightsOwner $i;
        getRightDescription $znak
        # echo $znak
    done
}
function getOwner(){
    owner=$(stat -f '%Su' "$1")
    echo "Owner: " $owner
}
function getLastModified(){
    last_modified=$(stat -f '%Sm' -t '%d.%m.%Y' "$1")
    echo "Last Modified: $last_modified"
}
getPath $1
getDirectoryRights $1
getOwner $1
getLastModified $1