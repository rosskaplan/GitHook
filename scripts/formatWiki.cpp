// Name : Brenda So
// Date : 12/16/2016
// Objective : After the database received the hash and the respective information, this script would be called and used to insert the commits related to the hash into the user database
// NOTE : pull first before doing this step
// NOTE : after this step we need to push
// sample line : [hash] brendabrandy made changes to file1, file2, file3 (with links) on (date, time): [commit msg]
// test case: rid, hash = 0f3fc14f9d281d29e3837771bb9047e720b84bb6 
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "utils.h"
#include "creds.h"
#include <mysql/mysql.h>
using namespace std;

MYSQL *mysql;
string rid;
// 
void addToPage(string commitLine, string tag){
    string q, purl, tid;
    int num_rows;
    MYSQL_RES *result;
    MYSQL_ROW row;
    fstream fs;
    q = "SELECT DISTINCT P.pid FROM page P WHERE P.rid="+rid+" and P.ptitle='"+tag+"';";
    if (mysql_query(mysql, q.c_str()) != 0){
        finish_with_error(mysql, "querying the database");
        exit(-1);
    }

    if ((result=mysql_store_result(mysql)) == NULL){
        finish_with_error(mysql, "storing result from database");
    }

    num_rows = mysql_num_rows(result);
    mysql_free_result(result);
    fs.open(("./wiki/"+tag+".md").c_str(), fstream::out|fstream::app);
    if (num_rows > 1){
        // something wrong
        cerr << "Error in the number of rows of page: " << num_rows << endl;
        exit(-1);
    }else if (num_rows == 0){
        // insert a new page into the database
        // first obtain purl and tid
        
        q = "SELECT DISTINCT W.wurl FROM wiki W WHERE W.repoid="+rid+";";
        if (mysql_query(mysql, q.c_str()) != 0){
            finish_with_error(mysql, "querying the database");
            exit(-1);
        }
        if ((result=mysql_store_result(mysql)) == NULL){
            finish_with_error(mysql, "storing result from database");
        }
        row = mysql_fetch_row(result);
        purl= ((string)row[0])+"/wiki/"+tag+".md";
        mysql_free_result(result);
        
        q = "SELECT T.tid FROM tags T WHERE T.tname='"+tag+"';";
        if (mysql_query(mysql, q.c_str()) != 0){
            finish_with_error(mysql, "querying the database");
            exit(-1);
        }
        if ((result=mysql_store_result(mysql)) == NULL){
            finish_with_error(mysql, "storing result from database");
        }
        row = mysql_fetch_row(result);
        tid = (string)row[0];
        mysql_free_result(result);
        q = "INSERT INTO page VALUES(NULL,'"+tag+"','"+purl+"',"+tid+","+rid+");";
        if (mysql_query(mysql, q.c_str())){
            finish_with_error(mysql, "inserting a page into database");
            exit(-1);
        }
        // as of now, make it so that new commits are added at the end
        // TODO: make the new commits the top;
        fs << "# Commits " << endl << endl;

    }

        
    fs << commitLine << endl << endl;
    fs.close();
    // first see whether the page to the tag exist
    // if it exist, 
    // if it doesn't, check database to verify it doesn't exist
    // and create a new page, and do the same as if it exist
}

// This function serves to parse a file url and obtain the filename
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

string formatLine(MYSQL_RES * result, string hasher){
    // obtain file and format it into a string like the sample line above
    stringstream ss; 
    string dtime, cmsg, fname;
    MYSQL_ROW row;
    int num_fields;
    // obtain the first result;
    row = mysql_fetch_row(result);
    cmsg = row[0];
    dtime = row[1];
    ss << "[" << dtime << "] ";
    fname = parseName(row[3]);
    ss << row[2] << " made changes to " << fname;
    while (row=mysql_fetch_row(result)){
        fname = parseName(row[3]);
        ss << ", " << fname;
    }
    ss << ": " << cmsg << endl;
    string s = ss.str();
    return s;

}

int main(int argc, char ** argv){
    string hasher, q, commit_line;
    hasher = argv[1];
    rid = argv[2];
    MYSQL_ROW row;
    MYSQL_RES * result;
    int num_fields;
    if ((mysql = mysql_init(NULL)) == NULL){
        finish_with_error(mysql, "Initializing mysql connection" );
        return 1;
    }

    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME, PORT, NULL, 0) == NULL){
        finish_with_error(mysql, "Establishing connection");
        return 1;
    }
    // make sure wiki exist in database?

    q = "SELECT C.cmsg, C.cdatetime, C.uname, F.furl FROM commits C, file F WHERE C.hash='"+hasher+"' and C.fid=F.fid;";
    // query the database and get all the commits you need, assuming an input argument is given
    if (mysql_query(mysql, q.c_str()) != 0){
        finish_with_error(mysql, "querying the database");
        return 1;
    }
    
    if ((result=mysql_store_result(mysql)) == NULL){
        finish_with_error(mysql, "storing result from database");
    }
    // format the commit into a string;
    commit_line = formatLine(result, hasher);
    
    mysql_free_result(result);
    q = "SELECT T.tname from tagged_changes C, tags T WHERE C.hash='"+hasher+"' and T.tid=C.tid;";
    if (mysql_query(mysql, q.c_str()) != 0){
        finish_with_error(mysql, "querying the database");
        return 1;
    }

    if ((result=mysql_store_result(mysql)) == NULL){
        finish_with_error(mysql, "storing result from database");
    }
    
    num_fields = mysql_num_fields(result);
    while (row=mysql_fetch_row(result)){
        addToPage(commit_line, row[0]);
    }
    mysql_close(mysql);
    return 0;
}
