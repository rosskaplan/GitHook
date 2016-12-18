// Name : Brenda So, Ross Kaplan
// Last Updated : 12/5/2016
// Updates : 12/5/2016 -- Brenda So   -- Made function to write a line to wiki page
// Updates : 12/4/2016 -- Ross Kaplan -- Made function to check whether user is in database

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <string.h>
#include <ctime>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <mysql/mysql.h>

using namespace std;

bool insertUser(string username);
int insertWikiURL(string repo_url);
int insertWorksIn(string username, string repo_url);
int insertTag(string tag);
int insertContainedTag(string tag, string repo_url);
int insertFiles(string file, string repo_url, string branch);
int insertTaggedChanges(string file, string hash);
int insertCommit(string message, string hash, string username, string file, string repo_url, string branch);
void finish_with_error(MYSQL *con, int n);

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

int main(int argc, char** argv) {

    string email, name, hash, temp, message, branch, q, tagtemp;
    vector<string> files, tags;
    int count = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            email = argv[i+1];
        }
        if (strcmp(argv[i], "-n") == 0) {
            //name = argv[i+1];
            ++i;
            while (strcmp(argv[i], "-b") != 0) { //while not equal
                name += argv[i];
                name+= " ";
                ++i;
            }
            name.pop_back(); //removes trailing space
        }
        if (strcmp(argv[i], "-b") == 0) {
            branch = argv[i+1];
            int openbracket = branch.find("[");
            int closebracket = branch.find("]");
            branch = branch.substr(openbracket+1, closebracket-openbracket-1);
        }
        if (strcmp(argv[i],"-r") == 0){
            rurl = argv[i+1];
        }
        if (strcmp(argv[i], "-m") == 0) {
            //temp = argv[i+1];
            ++i;
            while(strcmp(argv[i],"-f") != 0) { //While not equal
                temp += argv[i];
                temp += " ";
                ++i;
            }
            temp.pop_back(); //removes trailing space

            hash = temp.substr(0, 40);
            temp = temp.substr(41, temp.length()-41);
            for (int j = 0; j < temp.length(); j++) {
                if (temp[j] == '*')
                    ++count;
            }
            if (count % 2 == 1) {
                cerr << "There are an odd number of stars.  This commit will not be added to the wiki." << endl;
                return 1;
            }
            count = 0;
            for (int j = 0; j < temp.length(); j++) {
                if (temp[j] == '*') {
                    count++;
                    if (count % 2 == 1)
                        tagtemp.clear();
                    else if (!tagtemp.empty()) {
                        tags.push_back(tagtemp);
                        tagtemp.clear();
                    }
                } else {
                    tagtemp+=temp[j];
                }
            }
            message = tagtemp;
            message.erase(0,1);
        }
        int fileloc;
        string filetemp;
        string othertemp;
        if (strcmp(argv[i], "-f") == 0) {
            i++;
            while(argv[i]) {
                filetemp = argv[i++];
                while (filetemp.find("\n") != std::string::npos) {
                    fileloc = filetemp.find("\n");
                    othertemp = filetemp.substr(0, fileloc);
                    filetemp = filetemp.substr(fileloc+1, filetemp.length()-fileloc-1);
                    files.push_back(othertemp);
                }
                files.push_back(filetemp);
            }
        }
    }

    cout << "email: " << email << endl;
    cout << "name: " << name << endl;
    cout << "branch: " << branch << endl;
    cout << "hash: " << hash << endl;
    cout << "repo_url: " << rurl << endl;
    cout << "message: " << message << endl;
    
    for (int i = 0 ; i < files.size(); ++i){
        cout << "file: " << files[i] << endl;
    }
    string command;
    for (int i = 0; i < tags.size(); ++i) {
        cout << "tag: " << tags[i] << endl;
    }

    // Initializing MYSQL handler, which would be useful when initializing connection
    // with server
    if ((mysql = mysql_init(NULL)) == NULL){
        finish_with_error(mysql,1);
        return 1;
    }
    // Connecting to the database
    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME ,PORT,NULL,0) == NULL){
        finish_with_error(mysql,2);
        return 1;
    }
    //Error handling for wiki insertions
    int tempint = insertWikiURL(rurl);
    if (tempint == 2) {
        cerr << "User has not run installer script." << endl;
        exit(-1);
    } else if (tempint == 0) {
        cerr << "Failed to add repo url to database." << endl;
        exit(-1);
    }

    //Error handling for user insertion
    if (!insertUser(name)) {
        cerr << "Failed to add user to database." << endl;
        exit(-1);
    }

    if (!insertWorksIn(name, rurl)) {
        cerr << "Failed to add a user to a repo." << endl;
        exit(-1);
    }

    for (int i = 0; i < tags.size(); ++i) {
        if (!insertTag(tags[i])) {
            cerr << "Failed to add a tag to your repo." << endl;
            exit(-1);
        }
        if (!insertContainedTag(tags[i], rurl)) {
            cerr << "Failed to add a tag to contained tags." << endl;
            exit(-1);
        }
        if (!insertTaggedChanges(tags[i], hash)) {
            cerr << "Failed to add a tag to tagged changes." << endl;
            exit(-1);
        }
    }
    for (int i = 0; i < files.size(); i++) {
        if (!insertFiles(files[i], rurl, branch)) {
            cerr << "Failed to add a file to your repo." << endl;
            exit(-1);
        }
        if (!insertCommit(message, hash, name, files[i], rurl, branch)) {
            cerr << "Failed to add a commit to the database" << endl;
            exit(-1);
        }
    }
    q = "SELECT DISTINCT R.rid FROM repo R WHERE R.rurl='"+rurl+"';";
    if (mysql_query(mysql, q.c_str())){
        finish_with_error(mysql,3);
        return 0;
    }
    // Store the result of the query
    MYSQL_RES * result = mysql_store_result(mysql);
    if (result == NULL){
         finish_with_error(mysql, 4);
         return 0;
    }
    MYSQL_ROW row = mysql_fetch_row(result); 
    rid=(string)row[0];
    mysql_free_result(result);
    mysql_close(mysql);
    int pid;
    char ** newargs = (char**)malloc(4*sizeof(char*));
    string commandore="./scripts/formatWiki";
    newargs[0] = (char*)commandore.c_str();
    newargs[1] = (char*)hash.c_str();
    newargs[2] = (char*)rid.c_str();
    newargs[3] = NULL;
    switch(pid=fork()){
        case -1:
            cerr << "Cannot fork ./scripts/formatWiki: " << strerror(errno) << endl;
            break;
        case 0:
            if ((execvp(newargs[0], newargs)) == -1){
                cerr << "Cannot execute ./scripts/formatWiki: "  << strerror(errno) << endl;
            }
            break;
        default:
            if (wait(NULL) == -1){
                fprintf(stderr, "Cannot wait for ./scripts/formatWiki: %s\n", strerror(errno));    
            }
    }
    free(newargs);
    return 0;
}

int insertContainedTag(string tag, string repo_url) {

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

    temp = "SELECT rid FROM repo WHERE repo.rurl='"+repo_url+"';";
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
int insertCommit(string message, string hash, string username, string file, string repo_url, string branch) {

    string temp = "SELECT rid FROM repo WHERE repo.rurl='"+repo_url+"';";
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
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if (!row) {
        return 0;
    }
    string rid = (string)row[0];

    string tempurl;
    string git = ".git";
    if (strcmp(repo_url.substr(repo_url.length()-4).c_str(), git.c_str()) == 0) {
        tempurl = repo_url.substr(0, repo_url.length()-4)+"/blob/"+branch+"/"+file;
    } else {
        tempurl = repo_url+"/blob/"+branch+"/"+file;
    }

    temp = "SELECT file.fid FROM file WHERE file.furl='"+tempurl+"';";
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
    if (!row) {
        return 0;
    }
    string fid = (string)row[0];
    time_t now = time(0);

    tm *ltm = localtime(&now);

    string timetemp = to_string((ltm->tm_year)+1900)+"-"+to_string(ltm->tm_mon+1)+"-"+to_string(ltm->tm_mday)+" "+to_string(ltm->tm_hour)+":"+to_string(ltm->tm_min)+":"+to_string(1+ltm->tm_sec);

    temp = "INSERT INTO commits VALUES (NULL, '"+message+"', '"+fid+"', '"+hash+"', '"+timetemp+"','"+username+"', '"+rid+"');";

    if (mysql_query(mysql, temp.c_str())){
        finish_with_error(mysql,3);
        return 0;
    }

    // Free the memory that stores the result
    mysql_free_result(result);
    return 1;
}

int insertFiles(string file, string repo_url, string branch) {
    if (file.substr(file.length()-1, 1) == "\n")
        file = file.substr(0, file.length()-1);
    
    string tempurl;
    string git = ".git";
    if (strcmp(repo_url.substr(repo_url.length()-4).c_str(), git.c_str()) == 0) {
        tempurl = repo_url.substr(0, repo_url.length()-4)+"/blob/"+branch+"/"+file;
    } else {
        tempurl = repo_url+"/blob/"+branch+"/"+file;
    }
    string temp = "SELECT rid FROM repo WHERE repo.rurl='"+repo_url+"';";
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
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if (row) {
        string temp = "SELECT * FROM file WHERE file.furl='"+tempurl+"';";
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
        MYSQL_ROW row1;
        row1 = mysql_fetch_row(result);
        if (row1) {
            return 1;
        } else {
            temp = "INSERT INTO file VALUES (NULL,'"+tempurl+"', '"+(string)row[0]+"');";
            if (mysql_query(mysql, temp.c_str())){
                finish_with_error(mysql,3);
                return 0;
            }
        }
    } else {
        return 0;
    }
    // Free the memory that stores the result
    mysql_free_result(result);
    return 1;

}

// This function attemps to insert a page into the database into the database. If a page is successfully inserted, We need to add a page to the Wiki
int insertWorksIn(string username, string repo_url) {
    // Execute a query in the database
    string temp = "SELECT works_in.rid, works_in.uname FROM works_in, repo WHERE repo.rurl='"+repo_url+"'AND works_in.rid=repo.rid AND works_in.uname='"+username+"';";
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
        temp = "SELECT rid FROM repo WHERE repo.rurl='"+repo_url+"';";
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
        if (result) {
            temp = "INSERT INTO works_in (uname, rid) VALUES ('"+username+"', '"+(string)row[0]+"');";
            if (mysql_query(mysql, temp.c_str())){
                finish_with_error(mysql,3);
                return 0;
            }
        } else {
            return 0;
        }
            
    }
    // Free the memory that stores the result
    mysql_free_result(result);

    return 1;
}

// This function attemps to insert a page into the database into the database. If a page is successfully inserted, We need to add a page to the Wiki
int insertWikiURL(string repo_url){
    // Execute a query in the database
    string temp = "SELECT rid FROM repo WHERE repo.rurl='"+repo_url+"';";
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
    string repoid_result = (string)row[0];
    if (row[0]) {
        ///*+(string)row[0]*/
        
        temp = "SELECT repoid FROM wiki WHERE wiki.repoid='"+repoid_result+"';";
        if (mysql_query(mysql, temp.c_str())){
            finish_with_error(mysql,3);
            return 0;
        }

        MYSQL_RES * result = mysql_store_result(mysql);
        if (result == NULL){
            finish_with_error(mysql, 4);
            return 0;
        }
        row = mysql_fetch_row(result);
        if (row) {
        } else {
            string wurl = repo_url.erase(repo_url.length()-3)+"wiki.git";
            temp = "INSERT INTO wiki (repoid, wurl) VALUES ('"+repoid_result+"', '"+wurl+"');";
            if (mysql_query(mysql, temp.c_str())){
                finish_with_error(mysql,3);
                return 0;
            }
        }
    } else {
        return 2;
    }
// Free the memory that stores the result
    mysql_free_result(result);

    return 1;
}

// This function attempts to insert user into the database
bool insertUser(string username) {

    // Execute a query in the database
    if (mysql_query(mysql, "SELECT * FROM users;")){
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
    while ((row=mysql_fetch_row(result))){
        for (int i = 0 ; i < num_fields; i++){
            if (((string)row[i]).compare(username) == 0) {
                //Success: user already in database
                return 1;
            }
        }
    }
    char buffer[200];
    int length = sprintf(buffer, "INSERT INTO users (uname) VALUES ('%s');", username.c_str());
    buffer[length] = '\0';
    if (mysql_query(mysql, buffer)){
        finish_with_error(mysql,3);
        return 0;
    }
    
    num_fields = mysql_num_fields(result);
    // Show the result of the query
    
    // Free the memory that stores the result
    mysql_free_result(result);

    return 1;
}



void finish_with_error(MYSQL *con, int n){
    fprintf(stderr,"ERROR! %d %s\n", n, mysql_error(con));
    mysql_close(con);
}
