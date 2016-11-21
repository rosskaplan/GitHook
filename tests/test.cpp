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
#define UID "gordon"
#define PWD "gordon2"
#define PORT 3306
#define DBNAME "githook"

using namespace std;

void finish_with_error(MYSQL *con, int n){
    fprintf(stderr,"ERROR! %d %s\n", n, mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main(){
    
    MYSQL *handler;
    MYSQL *mysql;

    // Initializing MYSQL handler, which would be useful when initializing connection
    // with server
    if ((mysql = mysql_init(NULL)) == NULL){
        finish_with_error(mysql,1);
    }

    // Connecting to the database
    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME ,PORT,NULL,0) == NULL){
        finish_with_error(mysql,2);
    }

    cout << "Entered database!" << endl;

    // Execute a query in the database
    if (mysql_query(mysql, "SELECT * FROM tester;")){
        finish_with_error(mysql,3);
    }

    // Store the result of the query
    MYSQL_RES * result = mysql_store_result(mysql);
    if (result == NULL){
        finish_with_error(mysql, 4);
    }

    int num_fields = mysql_num_fields(result);
    // Show the result of the query
    MYSQL_ROW row;
    while ((row=mysql_fetch_row(result))){
        for (int i = 0 ; i < num_fields; i++){
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        cout << endl;
    }
    // Free the memory that stores the result
    mysql_free_result(result);
    // Close connection with databse
    mysql_close(mysql);

    return 0;
}
