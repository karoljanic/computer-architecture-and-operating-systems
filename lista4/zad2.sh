#!/bin/sh

init_variables () {
    timeCounter=0
    cpusNum=$(cat /proc/stat | grep "cpu" | wc -l)

    prevIdleArr=()
    prevNonIdleArr=()
    for i in $(seq 2 $(($cpusNum+2)))
    do
        prevIdleArr+=(0)
        prevNonIdleArr+=(0)
    done

    lastReceivedBytes=$(cat /proc/net/dev | grep "wlo1" | awk '{print $2}')
    lastTransmittedBytes=$(cat /proc/net/dev | grep "wlo1" | awk '{print $10}')

    startReceivedBytes=$(cat /proc/net/dev | grep "wlo1" | awk '{print $2}')
    startTransmittedBytes=$(cat /proc/net/dev | grep "wlo1" | awk '{print $10}')

    transmitted_series=()
    received_series=()
}

get_time () {
    upSeconds=$(cat /proc/uptime | awk '{print $1}')
    upSeconds=$(printf %.0f $upSeconds)
    secs=$(( $upSeconds % 60 ))
    mins=$(( ${upSeconds} / 60 % 60 ))
    hours=$(( ${upSeconds} / 3600 % 24 ))
    days=$(( ${upSeconds} / 86400 ))

    if [ "${days}" -ne "0" ]
    then
        echo "Time: ${days}d ${hours}h ${mins}m ${secs}s"
    elif [ "${hours}" -ne "0" ]
    then
        echo "Time: ${hours}h ${mins}m ${secs}s"
    elif [ "${mins}" -ne "0" ]
    then
        echo "Time: ${mins}m ${secs}s"
    else
        echo "Time: ${secs}s"
    fi
}

get_battery () {
    batLevel=$(cat /sys/class/power_supply/BAT0/capacity)
    echo "Battery: $batLevel%"
}

get_load_average () {
    loadAvg=$(cat /proc/loadavg)
    echo
    echo "Load Average:"
    echo " ->last 1 min: $(echo $loadAvg | awk '{print $1}')"
    echo " ->last 5 min: $(echo $loadAvg | awk '{print $2}')"
    echo " ->last 15 min: $(echo $loadAvg | awk '{print $3}')"
}

get_memory_info () {
    memTotal=$(cat /proc/meminfo | grep "^MemTotal" | awk '{print $2}')
    memFree=$(cat /proc/meminfo | grep "^MemFree" | awk '{print $2}')
    memAvailable=$(cat /proc/meminfo | grep "^MemAvailable" | awk '{print $2}')
    memUsage=$(($memTotal-$memAvailable))

    memTotal=$(echo "scale=2; $memTotal/1024/1024" | bc)
    memFree=$(echo "scale=2; $memFree/1024/1024" | bc)
    memAvailable=$(echo "scale=2; $memAvailable/1024/1024" | bc)
    memUsage=$(echo "scale=2; $memUsage/1024/1024" | bc)

    echo
    echo "Memory:"
    echo " ->total: $memTotal GB"
    echo " ->free: $memFree GB"
    echo " ->available: $memAvailable GB"
    echo " ->usage: $memUsage GB"
}

get_cpu_info () {
    cpuInfo=$(cat /proc/cpuinfo | grep "cpu MHz" | tr -d 'MHzcpu:')
    mapfile -t cpuInfoArray <<< $cpuInfo

    cpus=$(cat /proc/stat | grep "cpu")
    cpusArr=()
    for el in $cpus:
    do
        cpusArr+=($el)
    done

    echo
    echo "CPUs frequency and usage:"
    for i in $(seq 2 $cpusNum)
    do  
        currIdle=$((${cpusArr[$(($i*11-7))]} + ${cpusArr[$(($i*11-6))]}))
        currNonIdle=$((${cpusArr[$(($i*11-10))]} + ${cpusArr[$(($i*11-9))]} + ${cpusArr[$(($i*11-8))]} + ${cpusArr[$(($i*11-5))]} + ${cpusArr[$(($i*11-4))]} + ${cpusArr[$(($i*11-3))]}))
        total=$(($currIdle + $currNonIdle - ${prevIdleArr[$i]} - ${prevNonIdleArr[$i]}))
        idled=$(($currIdle - ${prevIdleArr[$i]}))
        perc=$(echo "scale=2; 100 * ($total - $idled) / $total" | bc | awk '{printf "%1.2f", $0}')
        freq="${cpuInfoArray[(($i-2))]}"
        freq=$(echo -n "${freq//[[:space:]]/}")
        echo " ->${cpusArr[$(($i*11-11))]}: $freq MHz - $perc%"
        prevIdleArr[$i]=$currIdle
        prevNonIdleArr[$i]=$currNonIdle
    done
}

bytes_format () {
    if (( $1 > 1048575 ))
    then
        val=$(echo "scale=2; $1 / 1048576" | bc)
        echo "$val MB"
    elif (( $1 > 1023 ))
    then
        val=$(echo "scale=2; $1 / 1024" | bc)
        echo "$val KB"
    else
        val=$(echo "scale=2; $1 / 1" | bc)
        echo "$val B"
    fi
}

draw_bytes_chart () {
    name=$1[@]
    data=("${!name}")

    width=$(tput cols)
    width=$((width/2))

    max=${ar[0]}
    for n in "${data[@]}" ; do
        ((n > max)) && max=$n
    done

    if (( max == 0 ))
    then
        max=1
    fi

    for i in "${data[@]}"
    do
        echo -n "|"
        len=$(echo "$i * $width / $max" | bc)
        empty=$((width - len))
        for j in $(seq $len)
        do  
            echo -n "#"
        done
        for j in $(seq $empty)
        do  
            echo -n " "
        done
        echo -n " " $(bytes_format $i)
        echo
    done
}

draw_received_bytes_chart () {
    receivedBytes=$(cat /proc/net/dev | grep "wlo1" | awk '{print $2}')
    dr=$(($receivedBytes - $lastReceivedBytes))
    avgR=$(echo "($receivedBytes - $startReceivedBytes)/$timeCounter" | bc)
    
    lastReceivedBytes=$receivedBytes

    if (( ${#received_series[@]} < 5 ))
    then
        received_series+=($dr)
    else
        new_received_series=()
        for i in $(seq 1 4)
        do
            new_received_series+=(${received_series[i]})
        done
        new_received_series+=($dr)

        received_series=()
        for el in "${new_received_series[@]}"
        do
            received_series+=($el)
        done
    fi

    echo
    echo "Received Bytes:"
    draw_bytes_chart received_series
    echo "Average Since Start Measuring: $(bytes_format $avgR)"
}

draw_transmitted_bytes_chart () {
    transmittedBytes=$(cat /proc/net/dev | grep "wlo1" | awk '{print $10}')
    dt=$(($transmittedBytes - $lastTransmittedBytes))
    avgT=$(echo "($transmittedBytes - $startTransmittedBytes)/$timeCounter" | bc)

    lastTransmittedBytes=$transmittedBytes

    if (( ${#transmitted_series[@]} < 5 ))
    then
        transmitted_series+=($dt)
    else
        new_transmitted_series=()
        for i in $(seq 1 4)
        do
            new_transmitted_series+=(${transmitted_series[i]})
        done
        new_transmitted_series+=($dt)

        transmitted_series=()
        for el in "${new_transmitted_series[@]}"
        do
            transmitted_series+=($el)
        done
    fi

    echo
    echo "Transmitted Bytes:"
    draw_bytes_chart transmitted_series
    echo "Average Since Start Measuring: $(bytes_format $avgT)"
}

init_variables

bet=1
while [ $bet=1 ]
do
    sleep 1
    timeCounter=$(($timeCounter+1))
    clear

    echo "System Information:"
    get_time
    get_battery
    get_load_average
    get_cpu_info
    get_memory_info
    draw_received_bytes_chart
    draw_transmitted_bytes_chart
done