#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int stateTransitions[19][11] = 
           /* 00 Start */                        {{1,  3,  1,  9,  9,  7,  0,  13, 14, 7, 17 },
           /* 01 Process Alpha */                 {1,  1,  1,  2,  2,  2,  2,  12, 2,  2, 12 },
           /* 02 Finish Alpha */                  {0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0 },
           /* 03 Process Int */                   {12, 3,  12, 4,  5,  12, 5,  12, 12, 12, 12},
           /* 04 Process Float */                 {12, 4,  12, 12, 6,  12, 6,  12, 12, 12, 12},
           /* 05 Finish Int */                    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
           /* 06 Finish Float */                  {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
           /* 07 Process Operator */              {12, 12, 12, 12, 12, 8,  8,  12, 8,  12, 12},
           /* 08 Finish Operator */               {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
           /* 09 Process Separator */             {12, 12, 12, 12, 12, 12, 11, 12, 12, 12, 12},
           /* 10 Finish Separator */              {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
           /* 11 End */                           {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
           /* 12 Error */                         {12, 12, 12, 12, 12, 12, 0,  12, 12, 12, 12},
           /* 13 Process Exclamation */           {2, 2,   2,  2,  2,  8,  2,  2,  2,  2,  12},
           /* 14 Start c Comment */               {7, 7,   7,  7,  7,  7,  7,  7,  7,  15, 12},
           /* 15 Process c Comment */             {15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 12},
           /* 16 Finish c Comment */              {15, 15, 15, 15, 15, 15, 15, 15, 0,  15, 12},
           /* Process str literal */              {17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18},
           /* Finish str literal */               {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};

string currentStr = "";
size_t currentState = 0;
vector<string> keywords = {"int", "float", "bool", "if", "else", "then", "endif", 
                           "while", "whileend", "do", "doend", "for", "forend", 
                           "input", "output", "and", "or", "function", "include",
                           "sstring", "fstream", "string", "vector", "using", 
                           "return", "size_t", "cout"};
ofstream output;
ifstream in;

size_t convertToIndex(char c)
{
    if (isalpha(c) || c == '_')
        return 0;
    if (isdigit(c))
        return 1;
    if (c == '$')
        return 2;
    if (c == '.')
        return 3;
    if (c == '\'' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' 
        || c == ']' || c == ',' || c == ':' || c == ';' || c == '\\')
        return 4;
    if (c == '+' || c == '-' || c == '=' || c == '>' || c == '|'
        || c == '<' || c == '>' || c == '%' || c== '#')
        return 5;
    if (c == ' ' || c == '\n' || c == '\t')
        return 6;
    if (c == '!')
        return 7;
    if (c == '/')
        return 8;
    if (c == '*')
        return 9;
    if (c == '"')
        return 10;
    output << "INDEXING ERROR\n";
    return -1;
};

void processAlpha(char c)
{
    currentStr += c;
}

void finishSeparator(char c)
{
    output << "SEPARATOR          =             " << currentStr << endl;
    currentState = 0;
    currentStr = "";
}

void finishOperator(char c)
{
    output << "OPERATOR           =             " << currentStr << endl;
    currentState = 0;
    currentStr = "";
}

void processOperator(char c)
{
    currentStr = c;
    char next = in.peek();
    if (c == '+')
    {
        if (next == '=' || next == '+')
        {
            currentStr += in.get();
        }
    }
    else if (c == '-')
    {
        if (next == '=' || next == '-')
            currentStr += in.get();
    }
    else if (c == '>' || c == '<' || c == '=' || c == '|')
    {
        if (next == c)
        {
            currentStr += in.get();
        }
    }
    finishOperator(c);
}

void finishAlpha(char c)
{
    vector<string>::iterator it = find(keywords.begin(), keywords.end(), currentStr);
    if (it != keywords.end())
        output << "KEYWORD            =             " << currentStr << endl;
    else
        output << "IDENTIFIER         =             " << currentStr << endl;
    currentState = 0;
    currentStr = "";
    if (convertToIndex(c) == 4 || convertToIndex(c) == 3)
    {
        currentStr += c;
        finishSeparator(c);
    }
    if (convertToIndex(c) == 5)
    {
        currentStr += c;
        processOperator(c);
    }
}

void processInt(char c)
{
    currentStr += c;
}

void processFloat(char c)
{
    currentStr += c;
}

void finishInt(char c)
{
    output << "INT                =             " << currentStr << endl;
    currentState = 0;
    currentStr = "";
    if (convertToIndex(c) == 4)
    {
        currentStr += c;
        finishSeparator(c);
    }
    if (convertToIndex(c) == 5)
    {
        currentStr += c;
        finishOperator(c);
    }
}

void finishFloat(char c)
{
    output << "REAL               =             " << currentStr << endl;
    currentState = 0;
    currentStr = "";
    if (convertToIndex(c) == 4)
    {
        currentStr += c;
        finishSeparator(c);
    }
    if (convertToIndex(c) == 5)
    {
        currentStr += c;
        finishOperator(c);
    }
}



void processExclamation(char c)
{
    char next = in.peek();
    if (next == '=')
    {
        currentStr = "!=";
        in.get();
        finishOperator(c);
    }
}

void processSeparator(char c)
{
    currentStr = c;
    finishSeparator(c);
}

void error(char c)
{
    currentStr = "";
    output << "ENCOUNTERED ERROR IN STATE " << currentState 
         << " WHILE PROCESSING '" << c << "'" << endl;
}

void processStringLiteral(char c)
{
    currentStr += c;
}

void finishStringLiteral(char c)
{
    currentStr += c;
    output << "STRING LITERAL     =             " << currentStr << endl;
    currentState = 0;
    currentStr = "";
}

void handleCurrentChar(char c)
{
    char next;
    size_t ind = convertToIndex(c);
    switch(currentState)
    {
    case 0: break;
    case 1: processAlpha(c);
            break;
    case 2: finishAlpha(c);
            break;
    case 3: processInt(c);
            break;
    case 4: processFloat(c);
            break;
    case 5: finishInt(c);
            break;
    case 6: finishFloat(c);
            break;
    case 7: processOperator(c);
            break;
    case 8: finishOperator(c);
            break;
    case 9: processSeparator(c);
            break;
    case 10: finishSeparator(c);
             break;
    case 12: error(c);
             break;
    case 13: 
             processExclamation(c);
             break;
    case 14: {
             if (in.peek() == '*')
             {
                 currentState = 15;
             }
             else
             {
                 processOperator(c);
             }
             break;}
    case 15: break;
    case 16: 
             if (in.peek() == '/')
             {
                 in.get();
                 currentState = 0;
             }
             break;
    case 17:
             processStringLiteral(c);
             break;
    case 18:
             finishStringLiteral(c);
             break;
    default: output << "INVALID CASE. CHAR=" << c << " in state " << currentState << "\n";
             break;
    }
}

void processFile(string fileName)
{
    output.open("output.txt");
    output << "\n\nTOKENS                          LEXEMES\n\n";
    ofstream o;
    o.open(fileName, ios_base::app);
    o << " ";
    o.close();
    in.open(fileName);
    char c;
    while (in.get(c))
    {
        currentState = stateTransitions[currentState][convertToIndex(c)];

        handleCurrentChar(c);
    }
    in.close();
    output.close();

    #ifdef linux
    system("cat output.txt");
    #endif

    #ifdef _WIN32
    system("type output.txt");
    #endif
}
