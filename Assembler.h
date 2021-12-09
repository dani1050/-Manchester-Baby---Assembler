//
// Created by danci on 01/12/2021.
// Edited by jerryyyx on 04/12/2021.
//

#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <array>
#include <algorithm>
#include <functional>


using namespace std;


class Assembler {
public:

    Assembler();
    vector<pair<string, int>> SymbolTable;
    vector<pair<string, string>> instructions;
    static vector<string> readFile(const string& f);
    static vector<string> removeComments(const vector<string>& commands);
    static string reverseString(string s);
    static string DecimalToBinary(int dec);
    static bool writeToFile(const vector<string>& code,const string& fileName);
    vector<string> convert(vector<string> uncommentedCode);
    string getInstruction(const string& instruction);
    void addVariableToSymbolTable(const string& variableName, int address);
    static string trim(const string& s);
    static string constructLine(string instruction , string operand );

};


#endif //ASSEMBLER_ASSEMBLER_H
