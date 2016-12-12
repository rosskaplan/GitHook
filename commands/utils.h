#include <string>
#include <fstream>
#include <iostream>
#include <mysql/mysql.h>

using namespace std;

// this sees whether the current user has access to a certain repository, if the
// function returns 0, the user does not have access, else the function returns repoid 
// in the database
string hasAccess(MYSQL *con, string uname, string repourl);

// this allows user to finish with error. The first argument, con, stores the mysql
// handler, while string s is the location where the error happens. For instance, 
// if there is an error at at connecting to mysql database, s would be "connecting
// to database"
void finish_with_error(MYSQL *con, string s);

// This function checks whether the repo url exist in the database. If the repo does
// not exist in our database, this function returns false. Else it returns true
bool existDB(MYSQL *con, string repo_link);

// This function checks whether user exist in the database. If the user does not
// exist in the database, this function returns false, Else it returns true
bool existUser(MYSQL *con, string uname);

