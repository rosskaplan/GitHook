// Name : Brenda So
// Date : 12/2/2016
// Objective : To change one wiki page
#include <iostream>
#include <string.h>
#include "creds.h"

#include <mysql/mysql.h>
using namespace std;

void finish_with_error(MYSQL *con, string s){
    cerr << "Error when " << s << " : " << mysql_error(con);
    mysql_close(con);
    exit(1);
}

int main(int argc, char** argv){
    for (int i = 0 ; i < argc; i++){
        // all the arguments
    }

    // first, open the correct file
    // then append message to file
    // then close the file


}
