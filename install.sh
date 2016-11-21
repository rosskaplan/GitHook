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

<<<<<<< HEAD
git remote add origin $repo_link

git --git-dir=.wiki add Home.md
git --git-dir=.wiki commit -m "first commit"
git --git-dir=.wiki remote add wiki $new_link
=======
git --git-dir=.wiki remote add wiki new_link
>>>>>>> 336e7f0593f9ed2bd5517414453395a1e20e26ea

# Step 3 : Set up post-commit into .git
mv post-commit .git/hooks/post-commit
chmod +x .git/hooks/post-commit
<<<<<<< HEAD

# Step 4 : Populate the database with all the necessary information, including
# repo name, username etc

./setup -u "$GIT_AUTHOR_EMAIL" -e "$GIT_AUTHOR_NAME"
=======
>>>>>>> 336e7f0593f9ed2bd5517414453395a1e20e26ea
