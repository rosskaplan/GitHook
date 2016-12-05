// Name : Brenda So
// Date : 12/5/2016
// Description : Describe a repo with its users, user emails, tags, what users contributed to what tags, repo url, and wiki url

#include <iostream>
#include <string.h>
#include <creds.h>

#include <mysql/mysql.h>

void finish_with_error(MYSQL *con, string s){
    cerr << "Error when " << s << " : " << mysql_error(con);
    mysql_close(con);
    exit(1); 
}

int main(int argc ,char ** argv){
    MYSQL *con;
    string rname;
    for (int i = 0 ; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            rurl = argv[i+1];
        }
    }

    if ((con = mysql_init(NULL) == NULL)){
        finish_with_error(con, "setting up MYSQL handler");
    }

    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con, "connecting to database");
    }
    // get the repo and repo id first
    q = "SELECT R.rname R.url FROM repo WHERE rurl=" + rurl;
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "getting repo id from "
    }
    // get the users who work in the repo
    // get the tags that is in the repo
    // match the tags to the users
}
