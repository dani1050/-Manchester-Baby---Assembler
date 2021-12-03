//
// Created by danci on 01/12/2021.
//


#include "Assembler.h"

Assembler::Assembler() {
    /*
     * A list of instructions that the Manchester Baby implements
     */
    instructions={
            {"JMP","000"},
            {"JRP","100"},
            {"LDN","010"},
            {"STO","110"},
            {"SUB","001"},
            {"SUB","101"},
            {"CMP","011"},
            {"STOP","111"}
    };
}

/*
 * Reads a file and returns its contents in a string vector where every string is a line in the file
 * f - name of the file
 * text - contents of the file
 */
vector <string> Assembler::readFile(string f) {
    vector<string> text;
    ifstream file(f);
    //checks if the file is accessible
    if(!file.good()){
        cout<<"Can not access the file"<<endl;
        return text;
    }
    string line;
    while(getline(file,line)){
        text.push_back(line);
    }
    file.close();
    return text;
}

/*
 * removes all the comments, empty lines and the start and end indicators from the program
 * commands - string vector where every string is a line of code
 * uncommentedCode - contains only the parts that are required for assembly
 */
vector<string> Assembler::removeComments(vector<string> commands) {
    vector<string> uncommentedCode;
    for(string line:commands){
        //removes the comments
        line=line.substr(0, line.find(';'));

        if(!line.empty()){
            //removes teh start and end indicators
            if(regex_search(line,regex("(START:)+"))){
                string s = "START:";
                line.erase(line.find(s),s.length());
            }else if(regex_search(line,regex("(END:)+"))){
                string s = "END:";
                line.erase(line.find(s),s.length());
            }
            //trims the lines
            line=line.substr(line.find_first_not_of(" "),line.find_last_not_of(" ")-line.find_first_not_of(" ")+1);
            uncommentedCode.push_back(line);
        }
    }
    return uncommentedCode;
}

/*
 * converts decimal numbers to binary numbers
 * dec - a decimal number that you want to convert to binary
 * s - the dec number converted into binary returned as an int
 */
int  Assembler::DecimalToBinary(int dec) {
    //checks if the number entered is larger than 0
    if (dec < 0) {
        throw std::invalid_argument("received negative value");
    }
    string s;
    while (dec > 0) {
        s = std::to_string(dec % 2) + s;
        dec = dec / 2;
    }
    return std::stoi(s);
}







