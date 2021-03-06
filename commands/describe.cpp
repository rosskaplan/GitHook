// Name : Brenda So
// Date : 12/5/2016
// Description : Describe a repo with its users, user emails, tags, what users contributed to what tags, repo url, and wiki url

#include <iostream>
#include <string.h>
#include <string>
#include "creds.h"
#include "utils.h"
#include <mysql/mysql.h>

using namespace std;

int main(int argc ,char ** argv){
    MYSQL *con;
    MYSQL_ROW row, row1;
    MYSQL_RES *result, *result1;
    bool detailed;
    string rurl, rid,q, uname;
    int access = 0;
    for (int i = 0 ; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            rurl = argv[i+1];
        }else if (strcmp(argv[i], "-u") == 0){
            if (strcmp(argv[i+1], "") == 0){
                cerr << "Your $GIT_AUTHOR_NAME is not set. Please set it to your github username" << endl;
            }else{
                uname = argv[i+1];
            }
        }else if (strcmp(argv[i], "-d") == 0){
            detailed = true;
        }
    }

    if ((con = mysql_init(NULL)) == NULL){
        finish_with_error(con, "setting up MYSQL handler");
    }

    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con, "connecting to database");
    }
    // Security check, see whether the user can access to the repository first
    rid = hasAccess(con, uname, rurl);
    if (((string)rid).compare("") == 0) {
        cerr << "You do not work in this repo -- you do not have access" << endl;
        mysql_close(con);
        exit(1);
    }
    // get the repo name, url and wiki url first
    q = "SELECT DISTINCT R.rname, R.rurl, WIKI.wurl, R.rid FROM repo R, wiki WIKI WHERE R.rid="+rid+" and WIKI.repoid = R.rid;";
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }

    result = mysql_store_result(con);
    if (result == NULL){
        finish_with_error(con, "storing result from query");
    }
    cout << "NEW" ;
    row = mysql_fetch_row(result);
    cout << "----------REPOSITORY SUMMARY----------"<< endl;
    cout << "Repository Name : " << row[0] << endl;
    cout << "Repository URL : " << row[1] << endl;
    cout << "Wiki URL : " << row[2] << endl;
    rid = row[3];
    mysql_free_result(result);
    cout << endl; 
    
    // get contributors
    q = "SELECT DISTINCT U.uname FROM users U, works_in W where W.rid=" + rid + " and W.uname = U.uname;";
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    result = mysql_store_result(con);
    q = "SELECT C.uname, count(DISTINCT C.hash) FROM commits C WHERE C.repo="+rid+" GROUP BY C.uname;";
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    cout << "----------CONTRIBUTORS----------" << endl;
    result1 = mysql_store_result(con);
    while (row1=mysql_fetch_row(result1)){
        cout << row1[0] << " : " << row1[1] << " commits" << endl;        
    }
    mysql_free_result(result);
    mysql_free_result(result1);
    
    // get tags
    cout << endl;
    if (detailed){
        q = "SELECT C.uname, T.tname, count(DISTINCT C.hash) from commits C, tags T, tagged_changes TC WHERE C.repo="+rid+" and C.hash=TC.hash and TC.tid=T.tid GROUP BY C.uname, T.tname;";
    }else{
        q = "SELECT DISTINCT T.tname FROM repo R, tags T, contained_tags CT where CT.rid="+rid+" and CT.tid=T.tid ORDER BY T.tname";
    }
    cout << "----------TAGS----------" << endl;
    mysql_query(con, q.c_str());
    result = mysql_store_result(con);
    if (detailed){
        while (row=(mysql_fetch_row(result))){
            cout << row[1] << " : " << row[0] << " - " << row[2] << " commits" << endl;  
        }
    }else{
        while (row=mysql_fetch_row(result)){
            cout << row[0] << endl;
        }
    }
    mysql_free_result(result);
    mysql_close(con);
    cout << endl;
    
    return 0;
}
