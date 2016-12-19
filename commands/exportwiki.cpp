// Name : Brenda So
// Date : 12/5/2016
// Description : export commits to a csv file, if -n is not specified, then the current url is assumed to be the thing to export from
// -f would be the output file name, if not the name would automatically be new.csv

#include <iostream>
#include <fstream>
#include <string.h>
#include "creds.h"
#include "utils.h"
#include <mysql/mysql.h>

using namespace std;

MYSQL *con;

int main(int argc, char ** argv){

    MYSQL_ROW row;
    MYSQL_RES * result;
    string rurl, rid,q;
    bool sortbyTag;
    string fname = "new.csv";
    string uname;

    for (int i = 0 ; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            rurl = argv[i+1];
        }else if (strcmp(argv[i], "-t") == 0){
            sortbyTag = true;
        }else if (strcmp(argv[i], "-f") == 0){
            fname = argv[i+1];
        }else if (strcmp(argv[i], "-u") == 0){
            if (strcmp(argv[i+1],"") == 0){
                cout << "Please enter your username : ";
                cin >> uname; // security issues?, same with other stuff
            }else{
                uname = argv[i+1];
            }
        }
    }

    ofstream outfile(fname.c_str());   // open the new.csv file

    if ((con = mysql_init(NULL)) == NULL){
        finish_with_error(con, "setting up MYSQL handler");
    }

    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con, "connecting to database");
    }

    // see whether the repo exist
    if (!existDB(con, rurl)){
        cerr << rurl << " does not exist in the DB" << endl;
        exit(0);
    }
    // see whether user has database
    if (!existUser(con, uname)){
        cerr << uname << "does not exist in the DB" << endl;
        exit(0);
    }
    // see whether user have access to the repo
    rid = hasAccess(con, uname, rurl);
    if (((string)rid).compare("") == 0){
        cerr << "You do not work in this repo -- you do not have access to " << rurl << endl;
        mysql_free_result(result);
        mysql_close(con);
        exit(1);
    }
    
    // query the database for the commits in the repo
    q = "SELECT DISTINCT C.cdatetime, C.uname, F.furl, C.hash, C.cmsg, T.tname FROM commits C, file F, tags T, tagged_changes TC WHERE C.repo="+rid+" AND F.rid=C.repo AND C.fid=F.fid AND TC.hash=C.hash AND TC.tid=T.tid;";
    
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }

    result = mysql_store_result(con);
    if (result == NULL){
        finish_with_error(con,"storing result");
    }
    int num_fields = mysql_num_fields(result);
    outfile << "datetime, uname, furl, hash, message, tag" << endl;
    while ((row=mysql_fetch_row(result))){
        for (int i = 0 ; i < num_fields; i++){
            outfile << row[i];
            if (i != num_fields-1){
                outfile << ",";
            }
        }
        outfile << endl;
    }
    mysql_free_result(result);
    outfile.close();
   return 0;
}
