#!/bin/sh
repo_link="$(git remote get-url origin)"

g++ describe.cpp -o describe -lmysqlclient
./describe -n "$repo_link" -u "$GIT_AUTHOR_NAME"    # note to self: prompt user to enter username if GIT author name is not set
