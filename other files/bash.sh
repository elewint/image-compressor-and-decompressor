#! /bin/sh

if [[ $1 = "v" ]]
then
    clear
    clear
    make
    valgrind --leak-check=full --track-origins=yes ./40image test.ppm > out
elif [[ $1 = "f" ]]
then
    # clear
    # clear
    make
    ./40image flowers.ppm > out
elif [[ $1 = "fv" ]]
then
    clear
    clear
    make
    valgrind --leak-check=full --track-origins=yes ./40image flowers.ppm > out
elif [[ $1 = "c" ]]
then
    clear
    clear
    make
    valgrind --leak-check=full --track-origins=yes ./40image -c flowers.ppm > out
elif [[ $1 = "d" ]]
then
    clear
    clear
    make
    valgrind --leak-check=full --track-origins=yes ./40image -d out > out2
elif [[ -z $1 ]]
then
    # clear
    # clear
    make
    ./40image test.ppm > out
    # ./40image test.ppm
fi
