#include <iostream>
#include <string.h>
#include "creds.h"


#include <mysql/mysql.h>
using namespace std;

// This program is used to setup necessary information for the database

void finish_with_error(MYSQL *con, string s){
   cerr << "Error when " << s << " : " << mysql_error(con); 
   mysql_close(con);
   exit(1); 
}

int main(int argc, char** argv){
    MYSQL *con;
    string email, uname, q;
    for (int i = 0 ; i < argc; i++){
        if (strcmp(argv[i],"-u") == 0){
            uname = argv[i+1];
        }
    }

    if ((con = mysql_init(NULL)) == NULL){
        finish_with_error(con, "setting up MySQL handler");
    }

    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con, "connecting to database");
    }
    // set up the username in the database. If user already exist, the user would be notified
    // else, insert the user into database
    q = "INSERT INTO users VALUES ('" + uname + "')";
    if (mysql_query(con, q.c_str()) != 0){
        cerr << "User is already in database" << endl;
    }else{
        cerr << uname << " inserted into database" << endl;
    }
    mysql_close(con);
    return 0;
}
