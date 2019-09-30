#include "lex.h"
#include <iostream>
using namespace std;

int main()
{
    vector<string> testFiles= {"input1.txt", "input2.txt", "input3.txt"};
    int n;
    while (true)
    {
        cout << "\nWelcome. There are 3 included files to test:\n\n"
            << "\t1. The source code for this program\n"
            << "\t2. A simple for loop\n"
            << "\t3. A simple Hello World program\n"
            << "Enter the number of the choice you would like to test.\n";
        cin >> n;
        processFile(testFiles[n - 1]);
    }
    return 0;
}