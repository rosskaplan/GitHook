# GitHook

Every project since the beginning of time has had insufficient documentation. Doesn't matter who you are, whether it's for school, professional or personal, irrelevant!  Documentation is annoying to write and can waste precious developer time. 

Introducing GITHOOK Documentation!  We'll run some scripts automatically each time you commit that make the documentation for you!  Just include tags in your commit message wrapped in asterisks and it'll create a wiki page on your github for all commits using that tag!

# Functionality
1. Autogenerating wiki : After the package is installed, whenever you enter a commit, a related comment about the commit would appear in the wiki!

3. Full export wiki : You can generate a csv file with the full export function, and then do your own analytics and manipulation with the csv file.

4. add / remove tag : add and remove tags if you've written something wrong!

5. Querying : You can query a specific repo that is in the database and specify user / tags / time / limit !

# Sample usage

## Autogenerate wiki during commits. 

`git add *` 

To commit with tags of your own choice:

`git commit -m "*Animations* *Internet Explorer Bugs* *Geolocation Bugs* Javascript plugins on all contact us pages now work for Internet Explorer 9!"`

`git push origin master`

## Full export wiki

To compile the command, please run:

`g++ ./.git/GitHook-master/commands/exportwiki.cpp ./.git/GitHook-master/commands/utils.cpp -lmysqlclient -o exportwiki`

To export a certain wiki (e.g. the wiki of https://github.com/brendabrandy/welcome-android.git), run:

`./exportwiki -r https://github.com/brendabrandy/welcome-android.git -f export.csv`

## Remove tag by hash

To compile the command, please run

`g++ ./.git/GitHook-master/scripts/removeTagByHash.cpp -o removeTagByHash -lmysqlclient`

To remove a tag from the hash 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa':

`./removeTagByHash aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa tagname`

## Add tag by hash

To compile the command, please run

`g++ ./.git/GitHook-master/commands/addTagByHash.cpp -o addTagByHash -lmysqlclient`

To add a tag to the hash 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa':

`./addTagByHash aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa tagname`

## Querying

During querying, you can input several options:

-u : specify user

-r : specify repo url

-t : specify tag

--from : specify from date

--to : specify to date

-l : specify the number of commits you want to show

To compiile the command, please run

`g++ ./.git/GitHook-master/commands/query.cpp ./.git/GitHook-master/commands/utils.cpp --std=c++11 -lmysqlclient -i query`

An example query is:

`./query -u brendabrandy -r https://github.com/brendabrandy/welcome-android.git --from 2016-12-16 -l 5`

To test the query function on a larger database, you can check out the 2048 repo and bootstrap repo. Some fun queries could be:

```
./query -u Gabriele\ Cirulli -r https://github.com/gabrielecirulli/2048 -t graphics -l 5
./query -u Anna\ Harren -r https://github.com/gabrielecirulli/2048 -l 1
./query -r https://github.com/twbs/bootstrap -u wangsai --from 2016-12-16 -l 10
```

# Required Packages and Dependencies

1. Install git
2. Install MYSQL C library
3. The system is tested on ubuntu 16.04


# Setup
1. Make sure you initialize the git repository and connected it with a github url that you own! If you do not own a github repo and still want to test a system out, you can fork a github repo from [https://github.com/brendabrandy/welcome-android](https://github.com/brendabrandy/welcome-android)

With the above link, your_name would be brendabrandy and your_repository would be welcome-android

```
git init
git remote add origin https://github.com/your_name/your_repository.git`
git pull origin master`
```


2. Download the zip folder of this github into the repository you want this plugin to work within
3. Place the zip in the .git folder of your local repository
4. Change directory with `cd ./.git`
5. Unzip the zip file with `unzip GitHook-master.zip`
6. Change directory with `cd ./GitHook-master` and delete .gitmodules and wiki/ with `rm .gitmodules` and `rm -r wiki/`
7. Run installation script `bash install/install.sh`
8. Go to the github webclient and go to the wiki tab. If it does not appear, go to settings and enable it.  Once in the wiki tab, click save wiki on the default first wiki.
9. Make sure the environment variable $GIT_AUTHOR_NAME is your github username!
10. You can start using GitHook now!

You should now be all setup!  All commits should come from your normal directory (the one with the .git in it!)
