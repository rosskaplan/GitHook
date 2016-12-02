#!/bin/sh

# Step 0 : compile everything

# Step 1: Obtain link of wiki
repo_link="$(git remote get-url origin)" # note to self: enhance by allowing different origins
new_link="${repo_link::-4}.wiki.git"
repo_name="$(git remote show -n origin | grep Fetch | cut -d. -d/ -f5-)"
repo_real_name="${repo_name::-4}"
author_name="$(git config --global user.name)"

cd ../..
# Step 2 : Initialize empty wiki repository
git submodule add $new_link wiki
git submodule init wiki
git submodule update wiki

cd .git

# Step 3 : Set up post-commit into .git
mv GitHook-master/install/post-commit ./hooks/post-commit
chmod +x ./hooks/post-commit

cd ../..

git ls-files # show all the files in git

cd GitHook-master
cd install

# Step 4 : Populate the database with all the necessary information, including
# reponame, username, wiki, files contained in github
# it is assumed that the github wiki is empty
make
./setup -u "$repo_link" -e "$author_name" -r "$repo_real_name" -w "$new_link"
