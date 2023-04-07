#!/bin/sh

pid_pattern="/proc/[0-9]+"

(
echo "PPID|PID|COMM|STATE|TTY|RSS|PGID|SID|OPEN_FILES"

for f in /proc/*
do
    if [ -d $f ] && [[ $f =~ $pid_pattern ]]
    then
        stat=$(cat $f/stat)
        statfile=${stat/(*)/"comms_trash"}
        
        ppid=$(echo $statfile | awk '{print $4}')
        pid=$(echo $statfile | awk '{print $1}')
        comm=$(grep -oP '\(\K.*?(?=\))' <<< $stat)
        state=$(echo $statfile | awk '{print $3}')
        tty=$(echo $statfile | awk '{print $7}')
        rss=$(echo $statfile | awk '{print $24}')
        pgid=$(echo $statfile | awk '{print $5}')
        sid=$(echo $statfile | awk '{print $6}')
        open_files=$(ls $f/fd | wc -l)

        echo "$ppid|$pid|$comm|$state|$tty|$rss|$pgid|$sid|$open_files"
    fi
done
) | column -t -s "|"