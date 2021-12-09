#include <iostream>
#include "Assembler.h"

int main() {
    Assembler a;
    try {
        Assembler::writeToFile(a.convert(a.removeComments(Assembler::readFile("BabyTest1.txt"))), "test.txt");
    }catch (exception& e) {
       cout<< e.what()<<endl;
    }
    return 0;
}
