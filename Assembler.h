//
// Created by danci on 01/12/2021.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

class Assembler {
public:
    int CI;
    Assembler();
    vector<pair<string, string>> instructions;
    static vector<string> readFile(string f);
    static vector<string> removeComments(vector<string> commands);
    int DecimalToBinary(int dec);
};


#endif //ASSEMBLER_ASSEMBLER_H
