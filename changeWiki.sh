<<<<<<< HEAD
#!/bin/sh

tag_name=""
message=""

while getopts "m:t:" opt; do
=======
#!/bin/bash

OPTIND = 1
tag_name=""
message=""

while getopts "mt:" opt; do
>>>>>>> 336e7f0593f9ed2bd5517414453395a1e20e26ea

    case "$opt" in
        t) tag_name=$OPTARG
            ;;
        m) message=$OPTARG
<<<<<<< HEAD
            ;;
    esac
done

if [ ! -f $tag_name.md ]; then
=======
    esac
done

if [! -f $tag_name.md ]; then
>>>>>>> 336e7f0593f9ed2bd5517414453395a1e20e26ea
    touch $tag_name.md
fi

message >> $tag_name.md

git wiki add $tag_name.md 
git wiki commit -m $message
git push wiki master


