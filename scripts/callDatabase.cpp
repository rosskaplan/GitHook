#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
using namespace std;

int main(int argc, char** argv) {

    string email, name, date, hash, temp, message;
    vector<string> files, tags;
    int count = 0;
    string tagtemp;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            email = argv[i+1];
        }
        if (strcmp(argv[i], "-n") == 0) {
            name = argv[i+1];
        }
        if (strcmp(argv[i], "-d") == 0) {
            date = argv[i+1];
        }
        if (strcmp(argv[i], "-m") == 0) {
            temp = argv[i+1];
            hash = temp.substr(0, 40);
            temp = temp.substr(41, temp.length()-41);
            for (int j = 0; j < temp.length(); j++) {
                if (temp[j] == '*')
                    ++count;
            }
            if (count % 2 == 1) {
                cout << "There are an odd number of stars.  This commit will not be added to the wiki." << endl;
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
        // see whether tag exist in the db
        // system(command.c_str());
    }

    return 0;
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



