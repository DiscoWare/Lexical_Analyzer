#include "lex.h"
#include <iostream>
using namespace std;

int main()
{
    cout << "Running program on the source code of the program itself.\n";
    processFile("input.txt");
    return 0;
}