## GitHook

Ever project since the beginning of time has had insufficient documentation. Doesn't matter who you are , whether it's for school, professional or personal, nothing. Documentation is annoying to write and can waste precious developer time. 

Introducing GITHOOK Documentation!  we'll run some scripts each time you commit that make the documentation for you!  Just include tags in your commit message wrapped in asterisks and it'll create a wiki page on your github for all commits using that tag!

## Functionality
1. Autogenerating wiki : After the package is installed, whenever you enter a commit, a related comment about the commit would appear in the wiki!

## Sample usage

To commit with tags of your own choice:
`git commit -m "*Animations* *Internet Explorer Bugs* *Geolocation Bugs* Javascript plugins on all contact us pages now work for Internet Explorer 9!"`

## Required Packages
1. Install mysql C library

## Setup

1. Download the zip folder of this github.
2. Place the zip folder in the .git folder of your local repository
3. Change directory with `cd ./.git`
4. Unzip the zip file with `unzip GitHook-master.zip`
5. Change directory with `cd ./GitHook-master` and run installation script `bash install/install.sh`
