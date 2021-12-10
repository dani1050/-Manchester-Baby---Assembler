#include <iostream>
#include "Assembler.h"

int main() {
    string answer;
    //MainMenu



    while (true) {
        cout<<"=============================================\n";
        cout << "Menu\n 1. Type Code\n 2. Assemble code from file\n 3. Quit\n";
        cout << "Enter your choice: ";
        cin >> answer;
        if (answer == "1") {
            Assembler a;
            vector<string> code;
            string line;
            cout << "Type exit\\n to exit" << endl;
            while (getline(cin, line)) {
                if (line == "exit")
                    break;

                code.push_back(line);
            }
            cout << "Write output to file[Y/N]: ";
            cin >> answer;
            while (true) {
                if (regex_search(answer, regex("^[Y]$|^[y]$|^[N]$|^[n]$"))) {
                    if (answer == "Y" || "y") {
                        cout << "Enter the filename: ";
                        cin >> answer;
                        try {
                            code = a.convert(a.removeComments(code));
                            Assembler::writeToFile(code, answer);
                            cout << "=============================================\n";
                            a.printVector(code);
                            break;
                        } catch (exception &e) {
                            cout << e.what() << endl;
                        }

                    }
                    try {
                        code = a.convert(a.removeComments(code));
                        cout << "=============================================\n";
                        a.printVector(code);
                    } catch (exception &e) {
                        cout << e.what() << endl;
                    }
                    break;
                } else {
                    cout << "Invalid input";
                }
            }

            break;
        } else if (answer == "2") {
            Assembler a;
            vector<string> code;
            string filename;
            cout << "Enter name of the file: ";
            cin >> filename;


            cout << "Write output to file[Y/N]: ";
            cin >> answer;
            while (true) {
                if (regex_search(answer, regex("^[Y]$|^[y]$|^[N]$|^[n]$"))) {
                    if (regex_match(answer, regex("^[Y]$|^[y]$"))) {
                        cout << "Enter the filename: ";
                        cin >> answer;
                        try {
                            code = a.convert(a.removeComments(Assembler::readFile(filename)));
                            Assembler::writeToFile(code, answer);
                            cout << "=============================================\n";
                            a.printVector(code);
                            break;
                        } catch (exception &e) {
                            cout << e.what() << endl;
                        }

                    }
                    try {
                        code = a.convert(a.removeComments(Assembler::readFile(filename)));
                        cout << "=============================================\n";
                        a.printVector(code);
                    } catch (exception &e) {
                        cout << e.what() << endl;
                    }
                    break;
                } else {
                    cout << "Invalid input";
                }
            }
        } else if (answer == "3") {
            exit(0);
        } else {
            cout << "Invalid choice" << endl;
        }


    }
    return 0;
}
