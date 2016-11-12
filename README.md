Every project since the beginning of time has had insufficient documentation.  Doesn't matter who you are, whether it's for school, professional or personal, nothing.  Documentation is annoying to write and can waste precious developer time.  

Introducing GITHOOK DOCUMENTATION!  We'll run some scripts each time you commit that make the documentation for you!  Just include tags in your commit message wrapped in asterisks and it'll create a wiki page on your github for all commits using that tag!

Example: `git commit -m "*Animations* *Internet Explorer Bugs* *Geolocation Bugs* Javascript plugins on all contact us pages now work for Internet Explorer 9!"`

Commit Handling and parsing to figure out which repository the commit is on, what "tags" were included, what the message was, what the commit hash was, the date & time, which user it was, which files were modified, etc.

After parsing, it's passed to a C++ script which is responsible for communicating with a MySQL server to check whether the tag exists, what files, messages, etc are related to that tag, and more.  

Updates the github's wiki with the returned data from the database to make documentation write itself!
