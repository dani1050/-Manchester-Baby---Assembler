#include <iostream>
#include "Assembler.h"

int main() {
    Assembler::removeComments(Assembler::readFile("BabyTest1.txt"));
    return 0;
}
