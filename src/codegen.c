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
char* expressions(struct treenode *node) {
    if (!node){
       return NULL;
    }

    if (node->nodeKind == INTEGER) {
        char* r = nextReg();
        printf("li %s, %d\n", r, node->val);
        return r;
    }

    //enter variable logic here

    if (node->numChildren == 3) {

        char* left  = expr(node->children[0]);
        char* right = expr(node->children[2]);
        char* result   = nextReg();

        int op = node->children[1]->val;

        if (op == ADD) {
            printf("add %s, %s, %s\n", result, left, right);
        }
        else if (op == SUB){
            printf("sub %s, %s, %s\n", result, left, right);
        }
        else if (op == MUL){
            printf("mul %s, %s, %s\n", result, left, right);
        }
        else if (op == DIV) {
            printf("div %s, %s\n", left, right);
            printf("mflo %s\n", result);
        }

    return res;

    }
}





//Condintionals here





//loops here


//calling function
void generateCode() {

}



