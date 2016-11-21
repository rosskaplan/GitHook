#!/bin/bash

OPTIND = 1
tag_name=""
message=""

while getopts "mt:" opt; do

    case "$opt" in
        t) tag_name=$OPTARG
            ;;
        m) message=$OPTARG
    esac
done

if [! -f $tag_name.md ]; then
    touch $tag_name.md
fi

message >> $tag_name.md

git wiki add $tag_name.md 
git wiki commit -m $message
git push wiki master


