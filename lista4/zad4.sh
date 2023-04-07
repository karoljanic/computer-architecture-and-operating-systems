#!/bin/sh

mapfile -d $'\n' files_with_the_same_sha256sums < <(find $1 -type f -exec sha256sum {} + | sort | uniq -w64 -dD)
size=${#files_with_the_same_sha256sums[@]}

resultFiles=()
for i in $(seq $size)
do
    iPrinted=0
    for j in $(seq $(($i+1)) $size)
    do
        sha1=$(echo ${files_with_the_same_sha256sums[i]} | awk '{print $1}')
        file1=$(echo ${files_with_the_same_sha256sums[i]} | awk '{print $2}')
        sha2=$(echo ${files_with_the_same_sha256sums[j]} | awk '{print $1}')
        file2=$(echo ${files_with_the_same_sha256sums[j]} | awk '{print $2}')
        
        if [[ $sha1 == $sha2 ]]
        then
            if cmp -s $file1 $file2
            then
                iPrinted=1
                resultFiles+=($file1)
                resultFiles+=($file2)
            else    
                resultFiles+=($file2)
            fi
            
        fi
    done
done

find ${resultFiles[@]} -exec du {} + | sort -r | awk '{print $2}'
