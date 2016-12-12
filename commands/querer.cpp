#include <iostream>
#include <stdlib.h>
#include "creds.h"

using namespace std;

void prompter(){
    cout << "You can do one of the following: " << endl;
    cout << "1) Search by name" << endl;
    cout << "2) Search by repo" << endl;
    cout << "3) Search by file" << endl;
    cout << "4) Search by tags" << endl;
    cout << "5) Exit" << endl;
    cout << "Please enter your choice: ";
}

void searchByName(){

}

void exitQuery(){
    cout << "Goodbye!" << endl;
    exit(0);
}

int main(){

    int choice;
    while (true){
        prompter();
        cin >> choice;
        switch(choice){
            case 1:
                searchByName();
                break;
            case 2:
                break;
            case 3 :
                break;
            case 4: 
                break;
            case 5:
                exitQuery();
                break;
            default:
                cout << choice << " is not a valid option. Please enter a valid option" << endl;
                break;
        }
    }


    return 9;
}
