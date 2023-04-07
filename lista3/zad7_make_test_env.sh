#!/bin/bash

if [ -d Test_Env ]
then
    rm -r Test_Env
fi

mkdir Test_Env
mkdir Test_Env/sUbdIrEctOrY

touch Test_Env/"    abc d"
touch Test_Env/"    ABC   DEF"
touch Test_Env/"-AbC aBc"
touch Test_Env/"--ABC DEF"
touch Test_Env/"--ABC          DEF"

touch Test_Env/sUbdIrEctOrY/" ABC DEF"