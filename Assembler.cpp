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

vector<array<int, 32>> Assembler::convertToBinaryVector(vector<string> uncommentedCode) {
    vector<array<int, 32>> BinaryCode;
    vector<pair<string,int>> variables;
    int vars = 0;
    
    //Counts how many variables there are
    for(int i=1; i < uncommentedCode.size(); i++){
        if (uncommentedCode[i].find("VAR") != std::string::npos){
            vars++;
            variables.push_back(std::make_pair(uncommentedCode[i].substr(0, uncommentedCode[i].find("VAR") - 5),i));
        }
    }
    
    //Code to put variables at the bottom of the binary code
    for(int i = 32-vars; i < 32; i++){
        int index = uncommentedCode.size() - vars;
        bool negative = false;
        int num = std::stoi(uncommentedCode[index].substr(uncommentedCode[index].find("VAR") + 2));

        int binaryNum[32] = { 0 };
        
        if (num < 0){
            num *= -1;
            negative = true;
        }
    
        int x = 0;
        while (num > 0) {
            binaryNum[x] = num % 2;
            num /= 2;
            x++;
        }   
        
        for (int z = 0; z < 32; z++){
            if (binaryNum[z] == 1){
                binaryNum[z] = 0;
            }
            else if(binaryNum[z] == 0){
                binaryNum[z] = 1;
            }
        }

        if (binaryNum[31] == 0){
            binaryNum[31] = 1;
        }
        else if(binaryNum[31] == 1){
            binaryNum[31] = 0;
        }

        for (int y = 0; y < 32; y++){
            BinaryCode[i][y] = binaryNum[y];
        }
    }

    //Sets the 13,14,15 slots in the 32bit binary according to the command in the machine code
    for(int i=1; i < uncommentedCode.size() - vars; i++){
        if (uncommentedCode[i].find("JMP") != std::string::npos){
            BinaryCode[i][13] = 0;
            BinaryCode[i][14] = 0;
            BinaryCode[i][15] = 0;
        }
        else if(uncommentedCode[i].find("JRP") != std::string::npos){
            BinaryCode[i][13] = 1;
            BinaryCode[i][14] = 0;
            BinaryCode[i][15] = 0;
        }
        else if(uncommentedCode[i].find("LDN") != std::string::npos){
            BinaryCode[i][13] = 0;
            BinaryCode[i][14] = 1;
            BinaryCode[i][15] = 0;
        }
        else if(uncommentedCode[i].find("STO") != std::string::npos){
            BinaryCode[i][13] = 1;
            BinaryCode[i][14] = 1;
            BinaryCode[i][15] = 0;
        }
        else if(uncommentedCode[i].find("SUB") != std::string::npos){
            BinaryCode[i][13] = 0;
            BinaryCode[i][14] = 0;
            BinaryCode[i][15] = 1;
        }
        else if(uncommentedCode[i].find("CMP") != std::string::npos){
            BinaryCode[i][13] = 0;
            BinaryCode[i][14] = 1;
            BinaryCode[i][15] = 1;
        }
        else if(uncommentedCode[i].find("STP") != std::string::npos){
            BinaryCode[i][13] = 1;
            BinaryCode[i][14] = 1;
            BinaryCode[i][15] = 1;
        }
        else{
            throw std::invalid_argument("Bad assembly code");
        }

        //Sets the 0,1,2,3,4 slots depending on the variables referenced in the machine code
        for (int y = 0; y < variables.size(); y++){
            if (uncommentedCode[i].find(variables[y].first) != std::string::npos){
                int decimal = variables[y].second;
                int position[5] = {0};
                int z = 0;
                while (decimal > 0) {
                    position[z] = decimal % 2;
                    decimal /= 2;
                    z++;
                }

                for (int m = 0; m < 5; m++){
                    BinaryCode[i][m] = position[m];
                }
            }
        }
    }

    return BinaryCode;
}





