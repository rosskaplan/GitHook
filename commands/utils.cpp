#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <typeinfo>
#include "creds.h"
#include "utils.h"

#include <mysql/mysql.h>

using namespace std;

void finish_with_error(MYSQL *con, string s){
    cerr << "Error when " << s << " : " << mysql_error(con) << endl;
    mysql_close(con);
    exit(1);
}

string hasAccess(MYSQL *con, string username, string repourl){
    string q = "SELECT DISTINCT R.rid FROM users U, works_in WI, repo R WHERE R.rurl='"+repourl+"' and U.uname='"+username+"' and U.uname=WI.uname and WI.rid=R.rid";
    MYSQL_ROW row;
    string access = "";
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    MYSQL_RES * result = mysql_store_result(con);
    if (result == NULL){
        finish_with_error(con, "obtaining result");
    }
    if (mysql_num_rows(result) != 0){
        row = mysql_fetch_row(result);
        access = (string)row[0];
    }
    return access;
}

string existDB(MYSQL *con, string repo_link){
    string q = "SELECT DISTINCT R.rid FROM repo R where R.rurl='"+repo_link+"'";
    MYSQL_ROW row;
    bool exist;
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    MYSQL_RES * result = mysql_store_result(con);
    if (result == NULL){
       finish_with_error(con, "obtaining result"); 
    }

    if (mysql_num_rows(result) == 0) return NULL;
    row = mysql_fetch_row(result);
    return (string)row[0];
}

bool existUser(MYSQL *con, string uname){
    string q = "SELECT DISTINCT U.uname FROM users U where U.uname='"+uname+"';";
    MYSQL_ROW row;
    bool exist;
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL){
        finish_with_error(con, "obtaining result");
    }
    if (mysql_num_rows(result) == 0) return false;
    return true;
  
}

string existTag(MYSQL *con, string tagname){
    string q = "SELECT DISTINCT T.tid FROM tags T where T.tname='"+tagname+"';";
    MYSQL_ROW row;
    bool exist;
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL){
        finish_with_error(con, "obtaining result");
    }
    if (mysql_num_rows(result) == 0) return NULL;
    row = mysql_fetch_row(result);
    return row[0];

}




