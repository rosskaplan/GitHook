#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <unistd.h>

using namespace std;

#define IP_ADDR "199.98.20.115"
#define UID "gordon"
#define PWD "gordon2"
#define PORT 3306
#define DBNAME "githook"

MYSQL *handler;
MYSQL *mysql;
MYSQL *mysql_temp;
string rurl;
string rid;

void finish_with_error(MYSQL *con, int n) {
    fprintf(stderr,"ERROR! %d %s\n", n, mysql_error(con));
    mysql_close(con);
}

int removeTagFromDatabase(string hash, string tag);
int main (int argc, char** argv) {

    cout << "Please make sure you are running from the main git directory (outside .git)" << endl;
    cout << "Please confirm with \"Y\", \"y\", or \"yes\"" << endl;
    string check;
    cin >> check;
    if (strcmp(check.c_str(), "Y") && strcmp(check.c_str(), "y") && strcmp(check.c_str(), "yes"))
        return 0;
    if (!argv[1]) {
        cerr << "Argv[1] should be a commit hash!" << endl;
        exit(-1);
    }
    string hash = argv[1];
    if (hash.length() != 40) {
        cerr << "Argv[1] should be a full commit hash of 40 characters!" << endl;
        exit(-1);
    }
    if (!argv[2]) {
        cerr << "Argv[2] should be a tag you want to remove off of your hash!" << endl;
        exit(-1);
    }
    string tag_file = argv[2];

    string filename = "wiki/"+tag_file+".md";
    string filename2 = "wiki/"+tag_file+"asldkfjalsdajsddkfjsfskfjassjdfka.md";
    ifstream intag(filename.c_str());
    ofstream outtag(filename2.c_str());
    string temp, line;
    bool havewritten = 0;
    while (getline(intag,line)) {

        if (line != "") {
            temp = line;
            temp.erase(0, 11);
            if (temp == hash) {
                line = "";
                while(line == "")
                    getline(intag, line);
            } else {
                havewritten = 1;
                outtag << line << endl << endl;
            }
        }
    }
    if (havewritten = 0) {
        if (remove(filename.c_str()) != 0) {
            cerr << "This was the only commit with this tag.  We tried to delete the file but failed!" << endl;
            exit(-1);
        }
    } else {
        int result = rename(filename2.c_str(), filename.c_str());
        if (result != 0) {
            cerr << "We tried to delete this line but this operation has failed!" << endl;
            exit(-1);
        }
    }
    remove(filename2.c_str());
    //Remove this row from the hash/tag database
    if (!removeTagFromDatabase(hash, tag_file)) {
        cerr << "Failed to remove the tag from the database properly" << endl;
    }
    system("scripts/removeTagByHash.sh");
    return 0;
}

int removeTagFromDatabase(string hash, string tag) {
    if ((mysql = mysql_init(NULL)) == NULL){
        finish_with_error(mysql,1);
        return 0;
    }
    // Connecting to the database
    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME ,PORT,NULL,0) == NULL){
        finish_with_error(mysql,2);
        return 0;
    }
    string temp = "SELECT tagged_changes.tid FROM tagged_changes, tags WHERE tagged_changes.hash='"+hash+"' AND tags.tname='"+tag+"' AND tags.tid = tagged_changes.tid";

    if (mysql_query(mysql, temp.c_str())){
    finish_with_error(mysql,3);
        return 0;
    }
    // Store the result of the query
    MYSQL_RES * result = mysql_store_result(mysql);
    if (result == NULL){
        finish_with_error(mysql, 4);
        return 0;
    }

    int num_fields = mysql_num_fields(result);
    // Show the result of the query
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if (!row) {
        return 0;
    }
    string tid = (string)row[0];
    temp = "DELETE FROM tagged_changes where hash='"+hash+"' AND tid='"+tid+"';";

    if (mysql_query(mysql, temp.c_str())){
        finish_with_error(mysql,3);
        return 0;
    }
    temp = "SELECT tagged_changes.hash FROM tagged_changes WHERE tagged_changes.tid='"+tid+"';";

    if (mysql_query(mysql, temp.c_str())){
    finish_with_error(mysql,3);
        return 0;
    }
    // Store the result of the query
    result = mysql_store_result(mysql);
    if (result == NULL){
        finish_with_error(mysql, 4);
        return 0;
    }
    row = mysql_fetch_row(result);
    if (row) {
        return 1;
    } //Since we're still here, that means there were no results and the tag now must be removed from tags
    temp = "DELETE FROM page WHERE tid="+tid+";";
    if (mysql_query(mysql, temp.c_str())){
    finish_with_error(mysql,3);
        return 0;
    }
    temp = "DELETE FROM contained_tags WHERE tid="+tid+";";
    if (mysql_query(mysql, temp.c_str())){
    finish_with_error(mysql,3);
        return 0;
    }
    temp = "DELETE FROM tags WHERE tid="+tid+";";
    if (mysql_query(mysql, temp.c_str())){
    finish_with_error(mysql,3);
        return 0;
    }
    
    return 1;
}
