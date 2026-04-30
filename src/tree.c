#include<tree.h>
#include<strtab.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct treenode tree;

// the root of the ast
tree *ast;

tree *maketree(int kind) {
   //makes basic tree node without any values
   tree *node = (tree*)malloc(sizeof(tree));
   node->nodeKind = kind;
   node->numChildren = 0;
   node->val = 0;
   node->name = NULL;
   node->op = 0;
   node->parent = NULL;
   for (int i = 0; i < MAXCHILDREN; i++) {
      node->children[i] = NULL;
   }
   return node;
}
tree *maketreeWithVal(int kind, int val) {
   //makes a tree node and then adds a val to it
   tree *node = maketree(kind);
   node->val = val;
   return node;
}
tree *maketreeWithName(int kind, const char *name) {
   tree *node = maketree(kind);
   node->name = strdup(name);
   return node;
}
tree *maketreeWithOp(int kind, char op) {
   tree *node = maketree(kind);
   node->op = op;
   return node;
}

void addChild(tree *parent, tree *child) {
   //checks there is a parent and a child and then checks if the max
   //children have been reached if not it assigns the child to the 
   //parent node
   if (!parent || !child) {
      return;
   }
   if (parent->numChildren < MAXCHILDREN) {
      parent->children[parent->numChildren++] = child;
      child->parent = parent;
   }
}
void printAst(tree *root, int nestLevel) {
  // this function shouldn't be called with a NULL node
  // you should be able to remove this return check
  if (!root) return;
  // we print nestLevel*2 spaces, then we print the number 123
  // if nestLevel == 0 then there are 0 leading spaces
  // if nestLevel == 1 then there are 2 leading spaces
  // we can easily tab out print statements with this method
  printf("%*s", nestLevel * 2, "");

  switch(root->nodeKind) {
     case PROGRAM: 
        printf("program\n");
        break;
     case DECLLIST:
        printf("declList\n");
        break;
     case DECL:
        printf("decl\n");
        break;
     case VARDECL:
        printf("varDecl\n");
        break;
     case TYPESPEC:
        printf("typeSpecifier,%s\n", root->name);
        break;
     case FUNDECL:
        printf("funDecl\n");
        break; 
     case FORMALDECLLIST:
        printf("formalDeclList\n");
        break;
     case FORMALDECL:
        printf("formalDecl\n");
        break;
     case FUNCTYPENAME: 
        printf("funcTypeName\n");
        break;
     case FUNBODY:
        printf("funBody\n");
        break;
     case LOCALDECLLIST:
        printf("localDeclList\n");
        break;
     case STATEMENTLIST:
        printf("statementList\n");
        break;
     case STATEMENT:
        printf("statement\n");
        break;
     case COMPOUNDSTMT:
        printf("compoundStmt\n");
        break;
     case ASSIGNSTMT:
        printf("assignStmt\n");
        break;
     case CONDSTMT:
        printf("condStmt\n");
        break;
     case LOOPSTMT:
        printf("loopStmt\n");
        break;
     case RETURNSTMT:
        printf("returnStmt\n");
        break;
     case EXPRESSION:
        printf("expression\n");
        break;
     case ADDEXPR:
        printf("addExpr\n");
        break;
     case TERM:
        printf("term\n");
        break;
     case FACTOR:
        printf("factor\n");
        break;
     case ADDOP:
        printf("addop,%c\n", root->op);
        break;
     case MULOP:
        printf("mulop,%c\n", root->op);
        break;
     case RELOP:
    switch (root->val) {
        case LT:  printf("relop,<\n"); break;
        case LTE: printf("relop,<=\n"); break;
        case GT:  printf("relop,>\n"); break;
        case GTE: printf("relop,>=\n"); break;
        case OP_EQ:  printf("relop,==\n"); break;
        case OP_NEQ: printf("relop,!=\n"); break;
        default:  printf("relop\n"); break;
    }
    break;
     case FUNCCALLEXPR:
        printf("funcCallExpr\n");
        break;
     case ARGLIST:
        printf("argList\n");
        break;
     case VAR:
        printf("var\n");
        break;
     case ARRAYDECL:
        printf("arrayDecl\n");
        break;
     case INTEGER:
        printf("integer,%d\n", root->val);
        break;
     case IDENTIFIER:
        printf("identifier,%s\n", root->name);
        break;
     case CHAR_NODE:
        printf("char,%d\n", root->val);
        break;

     default:
        printf("NODE %d\n", root->nodeKind);
        break;
     
  }
   for (int i = 0; i < root->numChildren; i++) {
       printAst(root->children[i], nestLevel + 1);
   }
  // we can also print our variable spacing first ...
  // printf("%*s", nestLevel*2, "");
  // then print the node information
  // printf("%s\n", "addexpr");
  // then we print the child nodes
  // for i from zero to root->numchildren printast(root->child[i], nestlevel+1)
}
