#!/bin/sh
repo_link="$(git remote get-url origin)"

g++ describe.cpp -o describe -lmysqlclient
./describe -n "$repo_link"
