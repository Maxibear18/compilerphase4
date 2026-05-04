#include <stdio.h>
#include "tree.h"

static FILE *output = NULL;
char *expressions(struct treenode *node);
static void assignments(struct treenode *node);
static int labelC = 0;
int regCount = 0;
static void genNode(struct treenode *node);

static int next(void)
{
    return labelC++;
}

//Register logic here
char* nextReg() {
    char* r = malloc(10);
    snprintf(r, sizeof(r), "$t%d", regCount++);
    return r;
}

void resetRegs() {
    regCount = 0;
}



//Assignments here

static void assignments(struct treenode *node)
{
    char* rhs = expressions(node->children[1]);
    char* var = node->children[0]->children[0]->name;

    fprintf(output, "sw %s, %s\n", rhs, var);
}


//Expressions here
char* expressions(struct treenode *node) {
    if (!node){
       return NULL;
    }

    if (node->nodeKind == INTEGER) {
        char* r = nextReg();
        char* varName = node->children[0]->name;
        fprintf(output,"li %s, %d\n", r, node->val);
        return r;
    }

    if (node->numChildren == 1) 
    {
        return expressions(node->children[0]);
    }

    //enter variable logic here

    if (node->numChildren == 3) {

        char* left  = expressions(node->children[0]);
        char* right = expressions(node->children[2]);
        char* result = nextReg();

        int op = node->children[1]->val;

        if (op == ADD) {
            fprintf(output, "add %s, %s, %s\n", result, left, right);
        }
        else if (op == SUB){
            fprintf(output, "sub %s, %s, %s\n", result, left, right);
        }
        else if (op == MUL){
            fprintf(output, "mul %s, %s, %s\n", result, left, right);
        }
        else if (op == DIV) {
            fprintf(output, "div %s, %s\n", left, right);
            fprintf(output, "mflo %s\n", result);
        }

    return result;

    }
    return NULL;
}





//Condintionals here

static void conditional(struct treenode *node){
    int lElse = next();
    int lEnd = next();
    char *cond = expressions(node->children[0]);
    fprintf(output, "beq %s, $zero, L%d\n", cond, lElse);

    genNode(node->children[1]);
    fprintf(output, "j L%d\n", lEnd);
    fprintf(output, "L%d:\n", lElse);
    if(node->numChildren == 3)
    {
        genNode(node->children[2]);
    }
    fprintf(output, "L%d:\n", lEnd);
}


//loops here

static void genNode(struct treenode *node)
{
    if(!node) return;

    switch(node->nodeKind)
    {
        case PROGRAM:
        case DECLLIST:
        case FUNBODY:
        case COMPOUNDSTMT:
        case STATEMENTLIST:
            for(int i = 0; i < node->numChildren;i++)
            {
                genNode(node->children[i]);
            }
            break;
        case STATEMENT:
            genNode(node->children[0]);
            break;
        case ASSIGNSTMT:
            assignments(node);
            break;

        case EXPRESSION:
        case ADDEXPR:
        case TERM:
        case FACTOR:
            expressions(node);
            break;
        case CONDSTMT:
            conditional(node);
            break;
        default:
        for(int i = 0;i < node->numChildren; i++)
        {
            genNode(node->children[i]);
        }
        break;
    }
}

//calling function
void generateCode(struct treenode *root, int argc, char *argv[]) {
    output = fopen(argv[argc - 1], "w");
    if(!output)
    {
        perror("fopen");
        return;
    }

    fprintf(output, ".text\n");
    fprintf(output, ".globl main\n");
    fprintf(output, "main:\n");

    genNode(root);

    fclose(output);
}



