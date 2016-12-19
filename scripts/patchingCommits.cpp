#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include "utils.h"
using namespace std;
string rid="37";
MYSQL *mysql;

#define IP_ADDR "199.98.20.115"
#define UID "gordon"
#define PWD "gordon2"
#define PORT 3306
#define DBNAME "githook"


// This helps to make a link after each commit
void makeLink(ofstream& fs, string hasher){
    // need a global rid
    fs << " [link](";
    string q = "SELECT rurl FROM repo WHERE repo.rid=+"+rid+";";
    if (mysql_query(mysql, q.c_str()) != 0){
        finish_with_error(mysql, "querying the database");
        exit(-1);
    }

    MYSQL_RES *result;

    if ((result=mysql_store_result(mysql)) == NULL){
        finish_with_error(mysql, "storing result from database");
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    fs << (string)row[0] << "/commit/"<<hasher<<")";
    return;

}
 
int main(int argc, char ** argv){
    DIR *dir;
    struct dirent *ent;
    string dname = "/home/brenda/Documents/bootstrap/wiki/";
    string fname;
    ofstream out;
    ifstream in;
    string comment, nl4, nl1, nl2, nl3, commitstring;
    string hasher;
    string tempfile = "temp.md";

    if ((mysql = mysql_init(NULL)) == NULL){
        finish_with_error(mysql, "Initializing mysql connection" );
        return 1;
    }

    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(mysql, "Establishing connection");
        return 1;
    }


    if ((dir=opendir(dname.c_str())) != NULL){
        while ((ent=readdir(dir)) != NULL){
            fname = (string)ent->d_name;
            if (fname.compare("reverse.md") == 0 or fname.compare("Home.md") == 0){
                continue;
            }
            in.open((dname+fname).c_str(), ifstream::in);
            out.open((dname+tempfile).c_str());
            while (getline(in, comment)){
                // cout << comment << endl;
                // out << comment << nl1 << commitstring;
                hasher = comment.substr(comment.size()-41);
                out << comment;
                getline(in, comment);// get the newline
                out << endl;
                out << endl;
                getline (in, comment); // get the commit
                out << comment;
                makeLink(out, hasher);
                getline(in, comment); // get the newline
                out << endl;
                getline(in, comment); // get the comment
                out << endl;
                out << endl;
            }
            in.close();
            out.close();
            rename((dname+tempfile).c_str(), (dname+fname).c_str());
        }
        closedir(dir);
    }

}
