#include <iostream>
#include "Assembler.h"

int main() {
    Assembler a;
    Assembler::writeToFile(a.convert(a.removeComments(Assembler::readFile("BabyTest1.txt"))),"test.txt");

    return 0;
}
