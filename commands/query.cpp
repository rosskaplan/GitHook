// usernames
// repos
// tags
// dates
// limit on 10

#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <mysql/mysql.h>
#include <vector>
#include <string.h>
#include <getopt.h>
#include <sstream>
#include "utils.h"
#include "creds.h"

using namespace std;

void printRow(MYSQL_ROW row);
string parseName(string furl);

MYSQL * con;

int main(int argc, char ** argv){
    
    // variables and default options
    bool from = false, to = false, limit = false;
    int lim, index, num_fields;
    lim = INT_MAX;
    string res;
    MYSQL_ROW row;
    MYSQL_RES *result;
    string from_date, to_date, q;
    from_date = "";
    to_date = "";
    vector<string> repos = {};
    vector<string> tags = {};
    vector<string> users = {};
    vector<string> real_users;
    static struct option long_options[]{
        {"from",    required_argument, 0, 0},
        {"to",      required_argument, 0, 1},
    };
    char ch;
    // parse the commands
    if ((con = mysql_init(NULL)) == NULL){
        finish_with_error(con,"setting up MYSQL handler");
    }
    
    if (mysql_real_connect(con, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(con,"connecting to the database");
    }
    
    while ((ch=getopt_long(argc, argv, "u:r:t:l:",long_options, &index)) != -1){
        switch(ch){
            case 0: // specify from date
                from_date = optarg;
                from = true;
                // check whether from date is earlier than current date
                break;
            case 1: // specify to date
                to_date = optarg;
                to = true;
                // check whether to date is later than from date and earlier than current date
                break;
            case 'u':   // specify users, current user by default
                if (existUser(con, optarg)){
                    users.push_back(optarg);
                }else{
                    cerr << optarg << " does not exist in the database" << endl;
                }
                
                break;
            case 'r':   // specify repos, current repo by default
               if (strcmp((res=existDB(con, optarg)).c_str(), "") != 0){
                    repos.push_back(res);
                }else{
                    cerr << optarg << " does not exist in the database" << endl;
                }
                break;
            case 't':   // specify tags, empty by default
                if (strcmp((res=existTag(con, optarg)).c_str(), "") != 0){
                    tags.push_back(res);
                }else{
                    cerr << optarg << " does not exist in the database" << endl;
                }
                break;
            case 'l':   // specify limit, unlimited by default
                limit = true;
                lim = atoi(optarg);
                break;
            default:
                cerr << "git query is wrong!" << endl;
                return 0;
        }
    }
    // commit, author, hash, date and time, commit message, tags
    q = "SELECT DISTINCT C.uname, C.hash, C.cdatetime, C.cmsg FROM commits C, repo R, tagged_changes TC WHERE TC.hash = C.hash and  C.repo=R.rid";
    // add rids to select statement
    for (int i = 0 ; i < repos.size(); i++){
        if (i == 0){
            q += " AND ";
            q += "(";
        }
        q += "C.repo="+repos[i];
        if (i == repos.size()-1){
            q += ")";
            break;
        } 
        q += " OR ";
    }
    // add username to select statement
    for (int i = 0 ; i < users.size(); i++){
        if (i == 0){
            q += " AND ";
            q += "(";
        }
        q+="C.uname='"+users[i]+"'";
        if (i == users.size()-1){
            q += ")";
            break;
        }
        q += " OR ";
    }

    for (int i = 0 ; i < tags.size(); i++){
        if (i == 0){
            q += " AND ";
            q += "(";
        }
        q += "T.tid="+tags[i];
        if (i == tags.size()-1){
            q += ")";
            break;
        }
        q += " OR ";
    }
    if (from){
        q += " AND DATE(cdatetime) > '"+from_date+"'";
    }
    if (to){
        q += " AND DATE(cdatetime) < '"+to_date+"'";
    }
    q += " ORDER BY C.cdatetime DESC;";
    // add tids to select statement
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "Querying the database");
    }
    result = mysql_store_result(con);
    num_fields = mysql_num_fields(result);
    int count = 0;
    while ((row=mysql_fetch_row(result)) && count < lim){
        printRow(row);
        count += 1;
    }
    mysql_close(con);
    return 0;
}
string parseName(string furl){
    stringstream ss(furl);
    string item, res;
    vector<string> tokens;
    char delim='/';
    int counter = 7;
    while (getline(ss, item, delim) ){
        tokens.push_back(item);
    }
    for (int i = 7; i < tokens.size(); i++){
        res += "/"+tokens[i];
    }
    return res;
}


void printRow(MYSQL_ROW row){
    string hasher, msg, q;
    int num_rows;
    hasher= row[1];
    MYSQL_ROW row2;
    MYSQL_RES * result;
    q = "SELECT F.furl from file F, commits C where C.hash='"+hasher+"' and C.fid=F.fid;";
    msg = row[3];
    cout << "Commit  : "<< row[1] << endl;
    cout << "Author  : " << row[0] << endl;
    cout << "DateTime: " << row[2] << endl;
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "Querying the database");
    }
    result = mysql_store_result(con);
    cout << "Affected files: ";
    num_rows= mysql_num_rows(result);
    while (row2=mysql_fetch_row(result)){
        cout << parseName((string)row2[0]) << ", ";
    }
    cout << endl;
    q = "SELECT DISTINCT T.tname from tags T, commits C, tagged_changes TC  where C.hash=TC.hash and TC.tid=T.tid and C.hash='"+hasher+"';";
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "querying the database");
    }
    result = mysql_store_result(con);
    cout << "Tags: ";
    while (row2=mysql_fetch_row(result)){
        cout << "#"<<(string)row2[0] <<", ";
    }
    cout << endl;
    mysql_free_result(result);
    cout << "Commit Message : " << msg << endl;
    cout << endl; 
}



