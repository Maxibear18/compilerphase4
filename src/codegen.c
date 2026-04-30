#include <stdio.h>
#include "tree.h"

//Register logic here
int regCount = 0;

char* nextReg() {
    static char r[10]
    prinf(r, "$t%d", regCount++);
    return r;
}

void resetRegs() {
    regCount = 0;
}



//Assignments here




//Expressions here





//Condintionals here





//loops here


//calling function
void generateCode() {

}



