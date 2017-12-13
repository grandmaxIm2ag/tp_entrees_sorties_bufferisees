#!/bin/bash

i=1
nb_test=0
nb_ok=0
max=6000

touch test_format.txt
./test_format test_format.txt && nb_ok=$((nb_ok+1))
nb_test=$((nb_test+1))

echo -ne "tests : $i/$max           successfull : $nb_ok/$max\r"
    
for i in `seq 2 2000`
do
    touch generate_file.txt
    touch copied_file.txt
    ./generator generate_file.txt
    #cat main.c > generate_file.txt
    ./main generate_file.txt copied_file.txt
    diff generate_file.txt copied_file.txt > /tmp/tmp &&
        nb_ok=$((nb_ok+1))
    nb_test=$((nb_test+1))
    echo -ne "tests : $i/$max           successfull : $nb_ok/$max\r"
done

for i in `seq 2001 4000`
do
    touch generate_file.txt
    touch copied_file.txt
    ./generator generate_file.txt
    #cat main.c > generate_file.txt
    ./main_static generate_file.txt copied_file.txt
    diff generate_file.txt copied_file.txt > /tmp/tmp &&
        nb_ok=$((nb_ok+1))
    nb_test=$((nb_test+1))
    echo -ne "tests : $i/$max           successfull : $nb_ok/$max\r"
done

for i in `seq 4001 $max`
do
    touch generate_file.txt
    touch copied_file.txt
    ./generator generate_file.txt
    #cat main.c > generate_file.txt
    ./main_dyn generate_file.txt copied_file.txt
    diff generate_file.txt copied_file.txt > /tmp/tmp &&
        nb_ok=$((nb_ok+1))
    nb_test=$((nb_test+1))
    echo -ne "tests : $i/$max           successfull : $nb_ok/$max\r"
done

if [ $nb_test -eq $nb_ok ]
then
    echo 'test OK'
else
    echo 'test failed'
fi
