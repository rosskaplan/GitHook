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
    string uname, wurl, rurl, q, repo_name;
    string rid;
    for (int i = 0 ; i < argc; i++){
        if (strcmp(argv[i],"-e") == 0){
            // enter username
            if (strcmp(argv[i+1], "") == 0){
                cout << "Please enter your github username: " << endl;
                cin >> uname;
            }else{
                uname = argv[i+1];
            }
        }else if (strcmp(argv[i],"-r") == 0){
            // enter repo name
            repo_name = argv[i+1];
        }else if (strcmp(argv[i], "-u") == 0){
            // enter repo url
            rurl = argv[i+1];
            string git = ".git";
            if (strcmp(rurl.substr(rurl.length()-4).c_str(),git.c_str()) ==0)
                rurl=rurl.substr(0,rurl.length()-4);
        }else if (strcmp(argv[i], "-w") == 0){
            // enter wiki url
            wurl = argv[i+1];
        }
    }
    cout << "username : " << uname << endl;
    cout << "reponame: " << repo_name << endl;
    cout << "rurl : " << rurl << endl;
    cout << "wurl : " << wurl << endl;
    if ((con = mysql_init(NULL)) == NULL){
        finish_with_error(con, "setting up MySQL handler");
    }

    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con, "connecting to database");
    }
    // set up the username in the database. If user already exist, the user would be notified
    // else, insert the user into database
    q = "INSERT INTO users VALUES ('" + uname + "');";
    if (mysql_query(con, q.c_str()) != 0){
        cerr << "User is already in database" << endl;
    }else{
        cerr << uname << " inserted into database" << endl;
    }

    // setup repo and wiki in the database. If repo and wiki already exist, the user would be
    // notified; else insert the repo and wiki into database 

    q = "INSERT INTO repo VALUES('"+repo_name+"',NULL,'"+rurl+"');";
    if (mysql_query(con, q.c_str()) != 0){
        cerr << "Repo already exist in database" << endl;
    }else{
        cerr << repo_name << " inserted into database" << endl;
    }

    // query the database for repoid
    q = "SELECT rid FROM repo WHERE rname = '"+repo_name+"';";
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "getting repo id from database");
    }

    MYSQL_RES * result = mysql_store_result(con);
    if (result == NULL){
        finish_with_error(con, "storing result from repo id");
    }

    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    rid = row[0];
    q = "INSERT INTO wiki VALUES("+rid+",'"+wurl+"');";
    if (mysql_query(con,q.c_str()) != 0){
        cerr << "Wiki already exist in database" << endl;
    }else{
        cerr << wurl << " inserted into database" << endl;
    }

    // setup the files in the database. If the file already exist, the user would be notified;
    // else insert into the database
    //
    // NOTE : Files also needed to be inserted into the database at each commit
    // NOTE : And what do we do with pre-existing commits?


    mysql_close(con);
    return 0;
}
