#include <iostream>
#include <vector>
#include <sstream>
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

int insertContainedTag(string tag, string hash) {

    string temp = "SELECT tags.tid FROM tags WHERE tags.tname='"+tag+"';";
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
    string temp_tid = (string)row[0];
    temp = "SELECT rid FROM repo R, commits C WHERE R.rid=C.repo AND C.hash='"+hash+"';";
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
    rid = (string)row[0];
    if (row) {
        string temp_rid = (string)row[0];
        
        temp = "SELECT * FROM contained_tags WHERE contained_tags.tid='"+temp_tid+"' AND contained_tags.rid='"+temp_rid+"';";
        
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
        row = mysql_fetch_row(result);
        if (row) {
            return 1;
        }
        temp = "INSERT INTO contained_tags VALUES ('"+temp_tid+"', '"+temp_rid+"');";
        if (mysql_query(mysql, temp.c_str())){
            finish_with_error(mysql,3);
            return 0;
        }
    } else {
        return 0;
    }
    // Free the memory that stores the result
    mysql_free_result(result);
    return 1;
}



int insertTaggedChanges(string tag, string hash) {
    // check whether the tuple exists yet
    string temp = "SELECT tags.tid FROM tags WHERE tags.tname='"+tag+"';";
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
    temp = "INSERT INTO tagged_changes VALUES ('"+(string)row[0]+"','"+hash+"');";
    if (mysql_query(mysql, temp.c_str())){
        finish_with_error(mysql,3);
        return 0;
    }

    // Free the memory that stores the result
    mysql_free_result(result);
    return 1;
}
int insertTag(string tag) {

    string temp = "SELECT tags.tname FROM tags WHERE tags.tname='"+tag+"';";
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
    if (row) {
        return 1;
    } else {
        temp = "INSERT INTO tags VALUES ('"+tag+"', NULL);";
        if (mysql_query(mysql, temp.c_str())){
            finish_with_error(mysql,3);
            return 0;
        }

        // Store the result of the query
    }
    // Free the memory that stores the result
    mysql_free_result(result);
    return 1;
}

void addToPage(string commitLine, string tag, string hasher){
    string q, purl, tid;
    int num_rows;
    MYSQL_RES *result;
    MYSQL_ROW row;
    fstream fs;
    q = "SELECT DISTINCT P.pid FROM page P WHERE P.rid="+rid+" and P.ptitle='"+tag+"';";
    cout << q << endl;
    if (mysql_query(mysql, q.c_str()) != 0){
        finish_with_error(mysql, 1);
        exit(-1);
    }

    if ((result=mysql_store_result(mysql)) == NULL){
        finish_with_error(mysql, 2);
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
            finish_with_error(mysql, 1);
            exit(-1);
        }
        if ((result=mysql_store_result(mysql)) == NULL){
            finish_with_error(mysql, 2);
        }
        row = mysql_fetch_row(result);
        purl= ((string)row[0])+"/wiki/"+tag+".md";
        mysql_free_result(result); 
        q = "SELECT T.tid FROM tags T WHERE T.tname='"+tag+"';";
        if (mysql_query(mysql, q.c_str()) != 0){
            finish_with_error(mysql, 1);
            exit(-1);
        }
        if ((result=mysql_store_result(mysql)) == NULL){
            finish_with_error(mysql, 2);
        }
        row = mysql_fetch_row(result);
        tid = (string)row[0];
        mysql_free_result(result);
        q = "INSERT INTO page VALUES(NULL,'"+tag+"','"+purl+"',"+tid+","+rid+");";
        if (mysql_query(mysql, q.c_str())){
            finish_with_error(mysql, 3);
            exit(-1);
        }
        // as of now, make it so that new commits are added at the end
        // TODO: make the new commits the top;
    }

    fs << "[comment]: "<< hasher << endl << endl;
    fs << commitLine << endl << endl;
    fs.close();
    // first see whether the page to the tag exist
    // if it exist, 
    // if it doesn't, check database to verify it doesn't exist
    // and create a new page, and do the same as if it exist
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


int main (int argc, char** argv) {

    if ((mysql = mysql_init(NULL)) == NULL){
        finish_with_error(mysql,1);
        return 0;
    }
    // Connecting to the database
    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME ,PORT,NULL,0) == NULL){
        finish_with_error(mysql,2);
        return 0;
    }    
    
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
    string temp, line;
    MYSQL_RES *result;
    string q = "SELECT C.cmsg, C.cdatetime, C.uname, F.furl FROM commits C, file F WHERE C.hash='"+hash+"' and C.fid=F.fid;";
        // query the database and get all the commits you need, assuming an input argument is given
    if (mysql_query(mysql, q.c_str()) != 0){
        finish_with_error(mysql,1);
        return 1;
    }
    if ((result=mysql_store_result(mysql)) == NULL){
        finish_with_error(mysql, 2);
    }
    // add Tagname to Database if it is not already there
    if (!insertTag(tag_file)){
        cerr << "Failed to add a tag to your repo" << endl;
        exit(-1);
    }
    if (!insertContainedTag(tag_file, hash)){
        cerr << "Failed to add tags to repo" << endl;
        exit(-1);
    }
    if (!insertTaggedChanges(tag_file, hash)){
        cerr << "Failed to add tagged changes to database" << endl;
        exit(-1);
    }
    string commit_line = formatLine(result, hash);
    // add tagged_changes to Database
    // add contained_tags if it is not already there
    cout << "HAA" << endl; 
    mysql_free_result(result);
    addToPage(commit_line, tag_file, hash);
    system("scripts/removeTagByHash.sh");
    return 0;
}


