<<<<<<< HEAD
<<<<<<< HEAD
## GitHook
=======
>>>>>>> aefbfce592c0a9ef81e9568cae1a52e64e3e4de8
=======
## GitHook
>>>>>>> 336e7f0593f9ed2bd5517414453395a1e20e26ea
Every project since the beginning of time has had insufficient documentation.  Doesn't matter who you are, whether it's for school, professional or personal, nothing.  Documentation is annoying to write and can waste precious developer time.  

Introducing GITHOOK Documentation!  we'll run some scripts each time you commit that make the documentation for you!  Just include tags in your commit message wrapped in asterisks and it'll create a wiki page on your github for all commits using that tag!

<<<<<<< HEAD
<<<<<<< HEAD
## Sample usage

=======
>>>>>>> aefbfce592c0a9ef81e9568cae1a52e64e3e4de8
=======
## Sample usage

>>>>>>> 336e7f0593f9ed2bd5517414453395a1e20e26ea
Example: `git commit -m "*Animations* *Internet Explorer Bugs* *Geolocation Bugs* Javascript plugins on all contact us pages now work for Internet Explorer 9!"`

Commit handling to figure out which repository the commit is on, what "tags" were included, what the message was, what the commit hash was, the date & time, which user it was, which files were modified, etc.

After parsing, it's passed to a C++ script which is responsible for communicating with a MySQL server to check whether the tag exists, what files, messages, etc are related to that tag, and more.  

Updates the Github's wiki with the returned data from the database to make documentation write it by itself!

<<<<<<< HEAD
<<<<<<< HEAD
## Setup: 
run `bash ./install.sh`
=======
Setup: 

Run these commands: `mv post-commit .git/hooks/post-commit`
`chmod +x .git/hooks/post-commit`
>>>>>>> aefbfce592c0a9ef81e9568cae1a52e64e3e4de8
=======
## Setup: 
run `bash ./install.sh`
>>>>>>> 336e7f0593f9ed2bd5517414453395a1e20e26ea
