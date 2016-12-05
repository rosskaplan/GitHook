// Name : Brenda So
// Date : 12/5/2016
// Description : export commits to a csv file 

#include <iostream>
#include <string.h>
#include "creds.h"

#include <mysql/mysql.h>

void finish_with_error(MYSQL *con, string s){
    cerr << "Error when " << s << " : " << mysql_error(con) << endl;
    mysql_close(con);
    exit(1);
}

int main(int argc, char ** argv){

    MYSQL *con;
    MYSQL_ROW row;
    MYSQL_RES * result;
    string rurl, rid,q;

    for (int i = 0 ; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            rurl = argv[i+1];
        }
    }

    if ((con = mysql_init(NULL)) == NULL){
        finish_with_error(con, "setting up MYSQL handler");
    }

    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con, "connecting to database");
    }

    q = "SELECT"
 

   return 0;
}
