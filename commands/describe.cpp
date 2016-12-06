// Name : Brenda So
// Date : 12/5/2016
// Description : Describe a repo with its users, user emails, tags, what users contributed to what tags, repo url, and wiki url

#include <iostream>
#include <string.h>
#include "creds.h"

#include <mysql/mysql.h>

using namespace std;

void finish_with_error(MYSQL *con, string s){
    cerr << "Error when " << s << " : " << mysql_error(con) << endl;
    mysql_close(con);
    exit(1); 
}

int main(int argc ,char ** argv){
    MYSQL *con;
    MYSQL_ROW row;
    MYSQL_RES * result;
    string rurl, rid,q, uname;
    int access = 0;
    for (int i = 0 ; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            rurl = argv[i+1];
        }else if (strcmp(argv[i], "-u") == 0){
            if (strcmp(argv[i+1], "") == 0){
                cout << "Please enter your username : ";
                cin >> uname;
            }
        }
    }

    if ((con = mysql_init(NULL)) == NULL){
        finish_with_error(con, "setting up MYSQL handler");
    }

    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con, "connecting to database");
    }
    // Security check, see whether the user can access to the repository first
    q = "SELECT DISTINCT U.uname FROM users U, works_in WI, repo R WHERE R.rurl='"+rurl+"' and U.uname='"+uname+"' and U.uname=WI.uname and WI.rid=R.rid";
    
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    result = mysql_store_result(con);
    while(result == NULL){
        finish_with_error(con, "storing result from query");
    }
    access = mysql_num_rows(result);

    if (!access){
        cout << "You do not work in this repo -- you do not have access to " << rurl << endl;
        exit(1);
    }
    mysql_free_result(result);
    
    // get the repo name, url and wiki url first
    q = "SELECT DISTINCT R.rname, R.rurl, WIKI.wurl, R.rid FROM repo R, wiki WIKI WHERE R.rurl='"+rurl+"' and WIKI.repoid = R.rid;";
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }

    result = mysql_store_result(con);
    if (result == NULL){
        finish_with_error(con, "storing result from query");
    }
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
    mysql_query(con, q.c_str());
    cout << "----------CONTRIBUTORS----------" << endl;
    result = mysql_store_result(con);
    while (row=mysql_fetch_row(result)){
        cout << row[0] << endl;        
    }
    mysql_free_result(result);
    
    // get tags
    cout << endl;
    q = "SELECT DISTINCT T.tname FROM repo R, tags T, contained_tags CT where CT.rid="+rid+" and CT.tid=T.tid";
    cout << "----------TAGS----------" << endl;
    mysql_query(con, q.c_str());
    result = mysql_store_result(con);
    while (row=mysql_fetch_row(result)){
        cout << row[0] << endl;
    }
    mysql_free_result(result);
    mysql_close(con);
    cout << endl;
    
    return 0;
}
