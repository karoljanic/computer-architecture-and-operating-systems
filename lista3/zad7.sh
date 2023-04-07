#!/bin/bash

if [ ! -z $1 ] # if argument is not empty 
then 
    cd $1 # go to given by argument directory
fi

for file in * # for file in directory
do   
    if [ -f "$file" ] # if "$file" is file
    then
        mv -T -n -- "$file" "$( tr '[:upper:]' '[:lower:]' <<< "$file" )"
    fi
done

# mv -T -n : move with treat destination as a normal file and not override if file just exists
# tr '[:upper:]' '[:lower:]' : translate upper leteters to lower letters
