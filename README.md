## GitHook

Every project since the beginning of time has had insufficient documentation. Doesn't matter who you are, whether it's for school, professional or personal, irrelevant!  Documentation is annoying to write and can waste precious developer time. 

Introducing GITHOOK Documentation!  We'll run some scripts automatically each time you commit that make the documentation for you!  Just include tags in your commit message wrapped in asterisks and it'll create a wiki page on your github for all commits using that tag!

## Functionality
1. Autogenerating wiki : After the package is installed, whenever you enter a commit, a related comment about the commit would appear in the wiki!

2. Full import wiki

3. Full export wiki 

4. add / remove tag

5. Summary of github

## Sample usage

To commit with tags of your own choice:
`git commit -m "*Animations* *Internet Explorer Bugs* *Geolocation Bugs* Javascript plugins on all contact us pages now work for Internet Explorer 9!"`

## Required Packages
1. Install mysql C library

## Setup

1. Download the zip folder of this github into the repository you want this plugin to work within
2. Place the zip in the .git folder of your local repository
3. Change directory with `cd ./.git`
4. Unzip the zip file with `unzip GitHook-master.zip`
5. Change directory with `cd ./GitHook-master` and delete .gitsubmodules and wiki/ with `rm .gitsubmodules` and `rm -r wiki/`
6. Run installation script `bash install/install.sh`
7. Go to the github webclient and go to the wiki tab. If it does not appear, go to settings and enable it.  Once in the wiki tab, click save wiki on the default first wiki.

You should now be all setup!  All commits should come from your normal directory (the one with the .git in it!)

## Ideas for functionality

Access level for public repo vs private repo

