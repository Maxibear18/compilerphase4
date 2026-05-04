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
    snprintf(r, 10, "$t%d", regCount++);
    return r;
}

void resetRegs() {
    regCount = 0;
}


//Assignments here

static void assignments(struct treenode *node)
{
    if (node->numChildren < 2) 
    {
        expressions(node->children[0]);
        return;
    }
    char* rhs = expressions(node->children[1]);
    struct treenode* lhs = node->children[0];
    char* name = lhs->children[0]->name;
    if (lhs->numChildren == 2) {
        char *idx = expressions(lhs->children[1]);
        char *offset = nextReg();
        fprintf(output, "sll %s, %s, 2\n", offset, idx);
        fprintf(output, "sw %s, %s(%s)\n", rhs, name, offset);
        return;
    }
    fprintf(output, "sw %s, %s\n", rhs, name);
}


//Expressions here
char *expressions(struct treenode *node) {
    if (!node) return NULL;

    switch (node->nodeKind) {
        case INTEGER: {
            char *r = nextReg();
            fprintf(output, "li %s, %d\n", r, node->val);
            return r;
        }
        case CHAR_NODE: {
            char *r = nextReg();
            fprintf(output, "li %s, %d\n", r, node->val);
            return r;
        }
        case VAR: {
            char *r = nextReg();
            char *name = node->children[0]->name;
            if (node->numChildren == 2) {
            char *idx = expressions(node->children[1]);
            char *offset = nextReg();
            fprintf(output, "sll %s, %s, 2\n", offset, idx);  // offset = index * 4
            fprintf(output, "lw %s, %s(%s)\n", r, name, offset);
            return r;
            }
            fprintf(output, "lw %s, %s\n", r, name);
            return r;
        }
        case EXPRESSION:
            if(node->numChildren == 1)
            {
                return expressions(node->children[0]);
            }
            else if(node->numChildren == 3)
            {
                char* left = expressions(node->children[0]);
                char* right = expressions(node->children[2]);
                char* result = nextReg();
                int op = node->children[1]->val;
                if(op == LT)
                {
                    fprintf(output, "slt %s, %s, %s\n", result, left, right);
                }
                else if(op == GT)
                {
                    fprintf(output, "slt %s, %s, %s\n", result, right, left);
                }
                else if(op == LTE)
                {
                    fprintf(output, "sle %s, %s, %s\n", result, left, right);
                }
                else if(op == GTE)
                {
                    fprintf(output, "sle %s, %s, %s\n", result, right, left);
                }
                else if(op == OP_EQ)
                {
                    fprintf(output, "seq %s, %s, %s\n", result, left, right);
                }
                else if(op == OP_NEQ)
                {
                    fprintf(output, "sne %s, %s, %s\n", result, left, right);
                }
                return result;
            }
            return NULL;
        case ADDEXPR:
        case TERM:
        case FACTOR:
        case STATEMENT:
        case ASSIGNSTMT:
            if (node->numChildren == 1)
                return expressions(node->children[0]);
            if (node->numChildren == 3) {
                char *left   = expressions(node->children[0]);
                char *right  = expressions(node->children[2]);
                char *result = nextReg();
                int op = node->children[1]->val;
                if (op == ADD)
                    fprintf(output, "add %s, %s, %s\n", result, left, right);
                else if (op == SUB)
                    fprintf(output, "sub %s, %s, %s\n", result, left, right);
                else if (op == MUL)
                    fprintf(output, "mul %s, %s, %s\n", result, left, right);
                else if (op == DIV) {
                    fprintf(output, "div %s, %s\n", left, right);
                    fprintf(output, "mflo %s\n", result);
                }
                return result;
            }
            return NULL;
        default:
            if (node->numChildren == 1)
                return expressions(node->children[0]);
            return NULL;
    }
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



