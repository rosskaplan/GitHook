// Name : Brenda So
// Date : 11 Nov 2016
// Description : test script to connect to database, create a table, insert an entry to the table,
// and queries the table

#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <mysql/mysql.h>

// Definint macros on details to connect to database
#define IP_ADDR "199.98.20.115"
#define UID "root"
#define PWD "brenda2"
#define PORT 3306
#define DBNAME "githook"

using namespace std;

int main(){
    
    MYSQL *handler;
    MYSQL *mysql;

    // Initializing MYSQL handler, which would be useful when initializing connection
    // with server
    clock_t t;
    t = clock(); 
    if ((mysql = mysql_init(NULL)) == NULL){
        cout << "Error occurred when initializing MySQL handler : " << endl;
        exit(-1);
    }
    t = clock()-t;
    cout << (float)t/CLOCKS_PER_SEC << endl;
    t = clock();
    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME ,PORT,NULL,0) != NULL){
        cout << "Error occurred when connection to database" << endl;
        cout << "ERROR CODE : " << mysql_errno(mysql) << ": "<< mysql_error(mysql) << endl;
        exit(-1);
    }else{
        cout << "Entered database!" << endl;
        if (mysql_query(mysql, "SELECT * FROM tester;") != 0){
            cout << "ERROR CODE : " << mysql_errno(mysql) << ": " << mysql_error(mysql) << endl;
            exit(-1);
        }
        mysql_close(mysql);
    }
    t = clock()-t;
    cout << (float)t/CLOCKS_PER_SEC << endl;

    return 0;
}
