#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int main()
{

    // string cin_string;
    // string getLine_string;

    // cout << "Enter a string to test:\n";
    // cin >> cin_string;
    // getline(cin, getLine_string);

    // cout << "What cin does to string:" << cin_string << endl;
    // cout << "What getLine does to string:" << getLine_string << endl;

    srand(time(0));
    // discard first value
    rand();
    int random_number = rand() % 10;

    cout << random_number << endl;

    return 0;
}