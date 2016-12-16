// Name : Brenda So
// Date : 12/16/2016
// Objective : To insert stuff into wiki after all the information is inserted int odatabse
// NOTE : pull first before doing this step
// NOTE : after this step we need to push
// sample line : [hash] brendabrandy made changes to file1, file2, file3 (with links) on (date, time): [commit msg]
// test case: hash = 0f3fc14f9d281d29e3837771bb9047e720b84bb6 
#include <iostream>
#include "utils.h"
#include "creds.h"
#include <mysql/mysql.h>
using namespace std;

MYSQL *mysql;

sting formatLine(result){
    // obtain filen
}

int main(int argc, char ** argv){
    string hasher, q, commit_line;
    hasher = argv[1];
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
    commit_line = formatLine(result);
    num_fields = mysql_num_fields(result);
    while (row=mysql_fetch_row(result)){
        for (int i = 0 ; i < num_fields; i++){
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        cout << endl;
    }
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
        for (int i = 0 ; i < num_fields; i++){
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        cout << endl;
    }
    
    // first check whether the page exist or not
    // if it doesn't exist locally, check whether it exist on the server
    // if it exist on the server, it means that git pull is not successful/
    // complete, so exit and ask the user to git pull manually
    // if it doesn't exist on the server, create a new page and name it
    // with the tag
    // file I/O to insert the message
    // do this for all the tags
    // and then exit, after we exit, we need to git pull/push the stuff now
}
