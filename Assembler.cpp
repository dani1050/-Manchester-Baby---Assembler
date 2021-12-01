//
// Created by danci on 01/12/2021.
//


#include "Assembler.h"

Assembler::Assembler() {
    CI=0;
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

vector <string> Assembler::readFile(string f) {
    vector<string> text;
    ifstream file(f);
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

vector<string> Assembler::assemble(vector<string> commands) {
    for(string line:commands){
        CI++;
        line=line.substr(0, line.find(';'));

    }
    return commands;
}

vector<string> Assembler::removeComments(vector<string> commands) {
    vector<string> uncommentedCode;
    for(string line:commands){
        line=line.substr(0, line.find(';'));

        if(!line.empty()){
            if(regex_search(line,regex("(START:)+"))){
                string s = "START:";
                line.erase(line.find(s),s.length());
            }else if(regex_search(line,regex("(END:)+"))){
                string s = "END:";
                line.erase(line.find(s),s.length());
            }
            line=line.substr(line.find_first_not_of(" "),line.find_last_not_of(" ")-line.find_first_not_of(" ")+1);
            uncommentedCode.push_back(line);
        }
    }
    return uncommentedCode;
}


int  Assembler::DecimalToBinary(int dec) {
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







