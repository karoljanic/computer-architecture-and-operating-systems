#! /bin/bash

CHARACTERS=('\u0020' '\u2588' '\033[32m\u0024\033[0m' '\033[47;34m#\e[0m' '\033[40;31m@\e[0m')

EMPTY_ID=0
WALL_ID=1
COIN_ID=2
PLAYER_ID=3
GHOST_ID=4
WALL_PROBABILITY=20 # %
COIN_PROBABILITY=30 # %

board_width=0
board_height=0
board_array=()

playerX=0
playerY=0
playerPoints=0

ghostsNumber=0
ghostsCoords=()

generate_board () { # initialize board with size $1 x $2 and initial value $3
    board_width=$1
    board_height=$2
    for i in $(seq 1 $(($1 * $2)))
    do
        board_array+=($3)
    done
}

get_board_value () { # returning value at position ($1, $2)
    idx=$(( $1 * $board_width + $2 ))
    echo ${board_array[idx]}
}

set_board_value () { # setting value $3 at position ($1, $2)
    idx=$(( $1 * $board_width + $2 ))
    board_array[$idx]=$3
}

get_random_int () { # getting random integer from range [$1, $2]
    echo $(($1 + $RANDOM % ($2 - $1 + 1) ))
}

generate_block () {
    set_board_value $1 $2 $3
    set_board_value $(( $board_height - $1 - 1 )) $2 $3
    set_board_value $1 $(( $board_width - $2 - 1 )) $3
    set_board_value $(( $board_height - $1 - 1)) $(( $board_width - $2 - 1)) $3
}

generate_walls () { # generating walls on board
    board_width_half=$(($board_width / 2 - 1))
    board_height_half=$(($board_height / 2 - 1))

    for i in $(seq 0 2 $board_height_half)
    do
        for j in $(seq 0 2 $board_width_half)
        do
            p=$(get_random_int 0 100)
            
            if [ $p -lt $WALL_PROBABILITY ]
            then
                generate_block $i $j $WALL_ID
                generate_block $(( $i + 1 )) $(( $j + 1 )) $WALL_ID
                generate_block $(( $i + 1 )) $j $WALL_ID
                generate_block $i $(( $j + 1 )) $WALL_ID
                
            else
                p=$(get_random_int 0 100)
                if [ $p -lt $COIN_PROBABILITY ]
                then
                    set_board_value $i $j $COIN_ID
                    set_board_value $(( $board_height - $i -1 )) $j $COIN_ID
                    set_board_value $i $(( $board_width - $j - 1 )) $COIN_ID
                    set_board_value $(( $board_height - $i - 1 )) $(( $board_width - $j -1 )) $COIN_ID                   
                fi           
            fi
        done
    done
}

generate_ghost () { # generating $1 ghosts
    ghostsNumber=$1
    ghostsCoords=()

    for i in $(seq 1 $1)
    do
        id=$WALL_ID

        while [[ $id -eq $WALL_ID ]] || [[ $id -eq $GHOST_ID ]] || [[ $id -eq $PLAYER_ID ]]
        do
            x=$(get_random_int 1 $(($board_height-1)) )
            y=$(get_random_int 1 $(($board_width-1)) )
            id=$(get_board_value $x $y)
        done

        set_board_value $x $y $GHOST_ID
        ghostsCoords+=($x)
        ghostsCoords+=($y)
    done
}

generate_player () { # generating player start position
    id=$WALL_ID

    while [[ $id -eq $WALL_ID ]] || [[ $id -eq $GHOST_ID ]] || [[ $id -eq $PLAYER_ID ]]
    do
        x=$(get_random_int 1 $(($board_height-1)) )
        y=$(get_random_int 1 $(($board_width-1)) )
        id=$(get_board_value $x $y)
    done

    playerX=$x 
    playerY=$y
    set_board_value $x $y $PLAYER_ID
}

draw_board () { # drawing board saved in board_array
    tput clear

    for i in $(seq 0 $(($board_height-1)) )
    do
        for j in $(seq 0 $(($board_width-1)) )
        do
            echo -n -e ${CHARACTERS[$(get_board_value $i $j)]}
        done
        if [ $i -ne $(($board_height-1)) ]
        then
            echo
        fi
    done
}

move_player () { # player move from ($1, $2) to ($3, $4)
    newId=$(get_board_value $3 $4)

    if [[ $newId -ne $WALL_ID ]]
    then
        if [[ $newId -eq $COIN_ID ]]
        then
            playerPoints=$(($playerPoints+1))
        elif [[ $newId -eq $GHOST_ID ]]
        then
            q=q
        fi

        tput cup $1 $2 
        echo -e -n ${CHARACTERS[$EMPTY_ID]}

        tput cup $3 $4
        echo -e -n ${CHARACTERS[$PLAYER_ID]}

        set_board_value $1 $2 $EMPTY_ID
        set_board_value $3 $4 $PLAYER_ID
        playerX=$3
        playerY=$4
    fi
}

update_ghost_positions () { # updating position of ghost saved at 2*$1 and 2*$1+1 positions
    x=$((2*$1))
    y=$((2*$1 + 1))
    gX=${ghostsCoords[x]}
    gY=${ghostsCoords[y]}
    
    if [[ $gX -eq $playerX ]]
    then
        newgX=$gX
    elif [[ $gX -gt $playerX ]]
    then
        newgX=$(mod $(($gX - 1)) $terminal_height)
    else
        newgX=$(mod $(($gX + 1)) $terminal_height)
    fi

    if [[ $gY -eq $playerY ]]
    then
        newgY=$gY
    elif [[ $gY -gt $playerY ]]
    then
        newgY=$(($gY-1))
    else
        newgY=$(($gY+1))
    fi

    id1=$(get_board_value $newgX $newgY)
    id2=$(get_board_value $gX $newgY)
    id3=$(get_board_value $newgX $gY)
    if [[ $id1 -eq $PLAYER_ID ]] || [[ $id2 -eq $PLAYER_ID ]] || [[ $id3 -eq $PLAYER_ID ]]
    then
        q=q
    elif [[ $id1 -ne $WALL_ID ]]
    then
        newgX=$newgX
        newgY=$newgY
    elif [[ $id2 -ne $WALL_ID ]]
    then
        newgX=$gX
        newgY=$newgY
    elif [[ $id3 -ne $WALL_ID ]]
    then
        newgX=$newgX
        newgY=$gY
    fi

    tput cup $gX $gY
    echo -e -n ${CHARACTERS[$EMPTY_ID]}

    tput cup $newgX $newgY
    echo -e -n ${CHARACTERS[$GHOST_ID]}

    set_board_value $gX $gY $EMPTY_ID
    set_board_value $newgX $newgY $GHOST_ID
    ghostsCoords[$x]=$newgX
    ghostsCoords[$y]=$newgY
}

update_ghosts_positions () { # updating positions of all ghosts
    for i in $(seq 0 $(($ghostsNumber - 1)) )
    do
        update_ghost_positions $i
    done
}

add_coin () { # adding new coin at random position
    p=$(get_random_int 0 100)
    x=$(get_random_int 1 $(($board_height-1)) )
    y=$(get_random_int 1 $(($board_width-1)) )
    id=$(get_board_value $x $y)

    if [[ $p -lt $COIN_PROBABILITY ]] && [[ $id -eq $EMPTY_ID ]]
    then
        set_board_value $i $j $COIN_ID   
        tput cup $x $y
        echo -e -n ${CHARACTERS[$COIN_ID]}     
    fi 
}

mod () { # calculating $1 mod $2 in proper way
    a=$1
    while [[ $a -lt 0 ]]
    do
        a=$(($a + $2))
    done

    echo $(($a % $2))
}


terminal_width=$(tput cols)
terminal_height=$(tput lines)

generate_board $terminal_width $terminal_height $EMPTY_ID
generate_walls
generate_ghost 5
generate_player
draw_board

tput civis

lastTime=$(date +%s)
while [[ $q != q ]]
do
    read -n 1 -s -t 1 q
    case "$q" in 
        [aA] ) move_player $playerX $playerY $playerX $(mod $(($playerY - 1)) $terminal_width);; 
        [dD] ) move_player $playerX $playerY $playerX $(mod $(($playerY + 1)) $terminal_width);; 
        [sS] ) move_player $playerX $playerY $(mod $(($playerX + 1)) $terminal_height) $playerY;; 
        [wW] ) move_player $playerX $playerY $(mod $(($playerX - 1)) $terminal_height) $playerY;; 
    esac

    currentTime=$(date +%s)
    dt=$(($currentTime - $lastTime))

    if [[ $dt -ge 1 ]]
    then
        update_ghosts_positions
        add_coin
        lastTime=$currentTime
    fi
done

tput clear
tput cnorm

echo "Game over. Your final score: $playerPoints"
