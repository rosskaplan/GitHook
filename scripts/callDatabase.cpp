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
#include <time.h>

#include <mysql/mysql.h>

using namespace std;

bool insertUser(string username);
void finish_with_error(MYSQL *con, int n);

#define IP_ADDR "199.98.20.115"
#define UID "gordon"
#define PWD "gordon2"
#define PORT 3306
#define DBNAME "githook"

MYSQL *handler;
MYSQL *mysql;
string rurl;

int main(int argc, char** argv) {

    string email, name, date, hash, temp, message;
    vector<string> files, tags;
    int count = 0;
    string tagtemp;

    // Initializing MYSQL handler, which would be useful when initializing connection
    // with server
    if ((mysql = mysql_init(NULL)) == NULL){
        finish_with_error(mysql,1);
        return 0;
    }
    // Connecting to the database
    if (mysql_real_connect(mysql, IP_ADDR, UID, PWD, DBNAME ,PORT,NULL,0) == NULL){
        finish_with_error(mysql,2);
        return 0;
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            email = argv[i+1];
        }
        if (strcmp(argv[i], "-n") == 0) {
            //name = argv[i+1];
            ++i;
            while (strcmp(argv[i], "-d") != 0) { //while not equal
                name += argv[i];
                name+= " ";
                ++i;
            }
            name.pop_back(); //removes trailing space
        }
        if (strcmp(argv[i], "-d") == 0) {
            date = argv[i+1];
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

            cout << "temp: \"" << temp << "\" " << endl;
            hash = temp.substr(0, 40);
            cout << "temp: \"" << temp << "\" " << endl;
            temp = temp.substr(41, temp.length()-41);
            cout << "temp: \"" << temp << "\" " << endl;
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
        if (strcmp(argv[i], "-f") == 0) {
            i++;
            while(argv[i]) {
                files.push_back(argv[i++]);
            }
        }
    }

    cout << "email: " << email << endl;
    cout << "name: " << name << endl;
    cout << "date: " << date << endl;
    cout << "hash: " << hash << endl;
    cout << "message: " << message << endl;
    string wiki_msg;
    // Concantenate all the information to form an entry in the wiki
    wiki_msg =  date + "\t" + name + " changed ";
    for (int i = 0 ; i < files.size(); ++i){
        cout << "file: " << files[i] << endl;
        wiki_msg += files[i] + " ";
    }
    wiki_msg += ": "+ message;
    string command;
    for (int i = 0; i < tags.size(); ++i) {
        cout << "tag: " << tags[i] << endl;
        void insertPage(tags[i]);

        // see whether tag exist in the db
        // if there isn't a tag, we have to add to database (contained_tags and page)

        // This part only concerns the wiki

    }

    //Time to insert all information into database as necessary

    if (!insertUser(name)) {
        cerr << "Failed to add user to database." << endl;
        exit(-1);
    }

    mysql_close(mysql);

    return 0;

}

// This function attemps to insert a page into the database into the database. If a page is successfully inserted, We need to add a page to the Wiki
bool insertPage(string tag){
    string q;
    // see whether a repo contains the tag yet
    q = "SELECT DISTINCT T.tname FROM repo R, tags T, contained_tags CT where CT.rid=R.rid and R.rurl='"+rurl+"' and CT.tid=T.tid and T.tname='"+tag+"';";
    if (mysql_query(mysql,q.c_str())){
        finish_with_error(mysql,3);
        return 0;
    }
    MYSQL_RES * result = mysql_store_result(mysql);
    if (result == NULL){
       finish_with_error(mysql,4);
       return 0;
    }
    int num_rows = mysql_num_rows(result);
    mysql_free_result(result);
    if (num_rows == 0){
        // see whether there is a tag is in DB
        q = "SELECT DISTINCT T.tname from tags T where T.tname='"+tags+"';";
        if (mysql_query(mysql,q.c_str())){
            finish_with_error(mysql, 3);
            return 0;
        }
        result = mysql_store_result(mysql);
        if (result == NULL){
            finish_with_error(mysql,4);
            return 0;
        }
        int num_rows2=mysql_num_rows(result);
        mysql_free_result(result);
        if (num_rows2 == 0){
            q = "INSERT INTO tags VALUES(NULL,'"+tag+"');";
            if (mysql_query(mysql, q.c_str())){
                finish_with_error(mysql,3);
                return 0;
            }
        }

        // get the tid and insert into contained_tags;
        // insert into page;

    }
    return 0;
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
                mysql_free_result(result);
                mysql_close(mysql);
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

// This function takes in a tag and makes a new page
void docuwrite(string tag, string hash, string name, string message, string date, ofstream outfile){
    
    outfile << "## " << tag << endl;
    outfile << endl;
    
    string final_msg = "["+hash+"]" + name + " : " + message;
    outfile << final_msg << endl;
    outfile << endl;

    final_msg = "Date :" + date;
    outfile << final_msg << endl;
    outfile << endl;

    outfile << endl;
    outfile.close();
}

void finish_with_error(MYSQL *con, int n){
    fprintf(stderr,"ERROR! %d %s\n", n, mysql_error(con));
    mysql_close(con);
}
