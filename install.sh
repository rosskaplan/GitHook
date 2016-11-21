# This script should be ran for installation. Helps set up the git repository

#!/bin/sh

# Step 1: Obtain link of wiki
repo_link="$(git remote get-url origin)" # note to self: enhance by allowing different origins
new_link="${repo_link::-4}.wiki.git"

# Step 2 : Initialize empty wiki repository
mv .git .temp
git init 
mv .git .wiki
mv .temp .git
rm -rf .temp

git --git-dir=.wiki remote add wiki new_link

# Step 3 : Set up post-commit into .git
mv post-commit .git/hooks/post-commit
chmod +x .git/hooks/post-commit
