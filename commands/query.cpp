// usernames
// repos
// tags
// dates
// limit on 10

#include <iostream>
#include <unistd.h>
#include <mysql/mysql.h>
#include <vector>
#include <string.h>
#include <getopt.h>
#include "utils.h"
#include "creds.h"

using namespace std;

MYSQL *con;

int main(int argc, char ** argv){
    
    // variables and default options
    bool from = false, to = false, limit = false;
    int lim, index, num_fields;
    string res;
    MYSQL_ROW row;
    MYSQL_RES *result;
    string from_date, to_date, q;
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
                    cerr << optarg << "does not exist in the database" << endl;
                }
                
                break;
            case 'r':   // specify repos, current repo by default
               if (strcmp((res=existDB(con, optarg)).c_str(), "") != 0){
                    repos.push_back(res);
                    cout << res << endl;
                }else{
                    cerr << optarg << "does not exist in the database" << endl;
                }
                break;
            case 't':   // specify tags, empty by default
                for (; optind < argc && *argv[optind] != '-'; optind++){
                    if (strcmp((res=existTag(con, argv[optind])).c_str(), "") != 0){
                        tags.push_back(argv[optind]);
                    }else{
                        cerr << argv[optind] << "does not exist in the database" << endl;
                    }
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
    cout << "HALO" << endl;
    // commit, author, hash, date and time, commit message, tags
    q = "SELECT C.uname, C.hash, C.cdatetime, C.cmsg FROM commits C, repo R WHERE C.repo=R.rid";
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
    cout << q << endl;
    // add tids to select statement
    // add time limit to select statement (from and to)
    // get the filenames somehow
    if (mysql_query(con, q.c_str()) != 0){
        finish_with_error(con, "Querying the database");
    }
    result = mysql_store_result(con);
    num_fields = mysql_num_fields(result);
    while (row=mysql_fetch_row(result)){
        printRow(row);
        for (int i = 0; i < num_fields; i++){
            printf("%s ", row[i]? row[i]:"NULL");
        }
        cout << endl;
    }
    mysql_close(con);
    return 0;
}

void printRow(MYSQL ROW){
    string hasher;
    hasher = row[1];
    cout << "commit: "<< row[1] << endl;
    cout << "Author: " << row[0] << endl;
    
}
