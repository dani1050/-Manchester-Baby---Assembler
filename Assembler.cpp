
#include "Assembler.h"

Assembler::Assembler() {
    /*
     * A list of instructions that the Manchester Baby implements
     */
    instructions = {
            {"JMP", "000"},
            {"JRP", "100"},
            {"LDN", "010"},
            {"STO", "110"},
            {"SUB", "001"},
            {"SUB", "101"},
            {"CMP", "011"},
            {"STP", "111"}
    };

}

/*
 * Reads a file and returns its contents in a string vector where every string is a line in the file
 * f - name of the file
 * text - contents of the file
 */
vector<string> Assembler::readFile(const string& f) {
    vector<string> text;
    ifstream file(f);
    //checks if the file is accessible
    if (!file.good()) {
        cout << "Can not access the file" << endl;
        return text;
    }
    string line;
    while (getline(file, line)) {
        text.push_back(line);
    }
    file.close();
    return text;
}

/*
 * removes all the comments and empty lines
 * commands - string vector where every string is a line of code
 * uncommentedCode - contains only the parts that are required for assembly
 */
vector<string> Assembler::removeComments(const vector<string> &commands) {
    vector<string> uncommentedCode;
    for (string line: commands) {
        //removes the comments
        line = line.substr(0, line.find(';'));
        line = trim(line);
        if (!line.empty())
            uncommentedCode.push_back(line);
    }
    return uncommentedCode;
}

/*
 * converts decimal numbers to binary numbers
 * dec - a decimal number that you want to convert to binary
 * s - the dec number converted into binary returned as an int
 */
string Assembler::DecimalToBinary(int dec) {
    //checks if the number entered is larger than 0
    if (dec < 0) {
        throw std::invalid_argument("received negative value: " + to_string(dec));
    }
    if (dec == 0) {
        return "0";
    }
    string s;
    while (dec > 0) {
        s = std::to_string(dec % 2) + s;
        dec = dec / 2;
    }
    return s;
}

bool Assembler::writeToFile(const vector<string> &code, const string &fileName) {
    ofstream File(fileName);
    string s;
    for (auto &i: code) {

        File << i + "\n";
    }
    File.close();
    return true;
}

/*
 * This function converts code to machine code
 * uncommentedCode - code without comments
 * return This is the machine code as a string vector
 */
vector<string> Assembler::convert(vector<string> uncommentedCode) {
    //initializing the string vector to store the code
    vector<string> binary;
    //This loop goes through the code line by line
    for (int i = 0; i < uncommentedCode.size(); ++i) {
        string line = uncommentedCode.at(i);
        //checks if the current line has a label
        if (regex_search(uncommentedCode.at(i), regex(":"))) {
            string variableName = uncommentedCode.at(i).substr(0, uncommentedCode.at(i).find(":"));
            //adds the label and address to the symbol table
            addVariableToSymbolTable(variableName, i);
            //removes the label from the code
            line.erase(line.find(variableName + ":"), (variableName + ":").length());
            line = trim(line);
        }
        //checks if the line is a variable
        if (regex_match(line.substr(0, line.find(' ')), regex("VAR"))) {
            //converts the variable to binary and reverses it
            string s = reverseString(DecimalToBinary(stoi(line.substr(line.find(' ') + 1))));
            s.insert(s.end(), 32 - s.length(), '0');
            //adds the variable to the store
            binary.push_back(s);
            continue;
        } else {
            //gets the binary value of the instruction
            string instruction = getInstruction(line.substr(0, line.find(' ')));
            //checks if the instruction is STP
            if (instruction == "111") {
                string s(13, '0');
                binary.push_back(s + instruction + s + "000");
                continue;
            }
            //TODO: Check if the number is small enough to fit
            string operand = line.substr(line.find(' ') + 1);
            operand = trim(operand);
            //checks if the operand is a label or not by checking if it includes any letter
            if (std::any_of(std::begin(operand), std::end(operand), ::isalpha)) {
                //This searches the SymbolTable for the right label if it finds the label and the address of the label is known than makes the operand equal to the right address
                for (auto &j: SymbolTable) {
                    if (operand == j.first && j.second != -1) {
                        binary.push_back(constructLine(instruction, DecimalToBinary(j.second)));
                    }
                }
                //If the label was not found than this sets the operand to temporary value until the addresses of the label is known
                if (operand == line.substr(line.find(' ') + 1)) {
                    addVariableToSymbolTable(operand, -1);
                    binary.push_back(constructLine(instruction, "*****"));
                }
                //if the label is an integer than it converts it to binary
            } else {
                binary.push_back(constructLine(instruction, DecimalToBinary(stoi(operand))));
            }
        }
    }
    //This goes through the code again looking for the missing label addresses
    for (int i = 0; i < uncommentedCode.size(); ++i) {
        string line = uncommentedCode.at(i);
        //checks if the line had a label with a missing address
        if (regex_search(binary.at(i), regex("\\*"))) {
            //This iterates through the SymbolTable looking for the missing address
            for (auto &j: SymbolTable) {
                //if the address is found than the temporary value is replaced
                if (j.first == line.substr(line.find_last_of(' ') + 1 && j.second != -1)) {
                    int varAddress = j.second;
                    string varAddressInBinary = reverseString(DecimalToBinary(varAddress));
                    varAddressInBinary.insert(varAddressInBinary.end(), 13 - varAddressInBinary.length(), '0');
                    binary.at(i).erase(0, 13);
                    binary.at(i) = varAddressInBinary + binary.at(i);
                    break;
                }
            }
        }
    }
    for (int i = 0; i < SymbolTable.size(); ++i) {
        if(SymbolTable.at(i).second==-1){
            //TODO: Change this to a more suitable exception
            throw invalid_argument("Variable "+SymbolTable.at(i).first+" is never declared");
        }
    }
    return binary;
}

//This function creates the binary row from the instruction and operand
//instruction - the instructions code in binary
//operand - operand in binary except when it is a temporary value
//return - the 32 bit binary row
string Assembler::constructLine(string instruction, string operand) {
    //reverses the operand
    operand = reverseString(operand);
    //adds trailing 0
    operand.insert(operand.end(), 13 - operand.length(), '0');
    instruction.insert(instruction.end(), 19 - instruction.length(), '0');
    return operand + instruction;
}

//this function adds a label and address to the Symbol table
void Assembler::addVariableToSymbolTable(const string &variableName, int address) {
    for (auto &i: SymbolTable) {
        //checks if the label is already part of the table
        if (variableName == i.first) {
            if(address!=-1 && i.second==-1) {
                cout << "Updated " + variableName + "s address in the SymbolTable to " + to_string(address) << endl;
                i.second = address;
            }else if(i.second>0){
                throw invalid_argument("Variable is already declared elsewhere "+variableName+" "+"line: "+ to_string(address)+"<->"+
                                                                                                                              to_string(i.second));
            }
            return;
        }
    }
    cout<<"Added "+variableName+" to the Symbol table with address "+ to_string(address)<<endl;
    SymbolTable.emplace_back(variableName, address);

}
//trims lines
string Assembler::trim(const string &s) {
    return s.empty() ? "" : s.substr(s.find_first_not_of(' '), s.find_last_not_of(' ') - s.find_first_not_of(' ') + 1);
}

//reverses string
string Assembler::reverseString(string s) {
    reverse(s.begin(), s.end());
    return s;
}
//finds the right instruction code to the instruction name
string Assembler::getInstruction(const string &instruction) {
    for (auto &i: instructions) {
        if (instruction == i.first) {
            return i.second;
        }
    }
    throw invalid_argument(
            "Invalid instruction the " + instruction + " is not part of the instruction set of the Manchester Baby");
}

