#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdio.h>

using namespace std;

int main (int argc, char** argv) {

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

    string filename = "../wiki/"+tag_file+".md";
    string filename2 = "../wiki/"+tag_file+"2.md";
    ifstream intag(filename.c_str());
    ofstream outtag(filename2.c_str());
    string temp, line;
    bool havewritten = 0;
    while (getline(intag,line)) {

        if (line != "") {
            temp = line;
            temp.erase(0, 11);
            if (temp == hash) {
                cout << "WE GOT A MATCH" << endl;
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
        if (result == 0) {
            cerr << "We tried to delete this line but this operation has failed!" << endl;
            exit(-1);
        }
    }
    return 0;
}
