
/* definitions */
%option debug
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../obj/y.tab.h"

 /* integers to track line and column numbers */
int scancol = 1;
int yycol = 1;
int scanlineno = 1;


 /* char pointer to pass error messages to driver.c */
char *scan_error_msg;

 /* TODO: functions to update line and column numbers */
void updateCol();
void countLines();

 /* TODO: function to process a string literal */
int processString();

%}

newline         (\r\n|\r|\n)
whitespace      [\t ]+
integer         0|[1-9][0-9]*
integerlead0    0[0-9]+
character       [^"\\\n]
charconst       \'([^\'\\\n]|\\[nt\"\\\'])\'
letter          [A-Za-z]
escape          \\.

 /* String can contain any characters between the double quotes */
 /* other than a newline or unescaped doublequotes */

string          \"({character}|{escape})*\"
untermstring_eof    \"({character}|{escape})*

 /* If the end quote is not found in the same line the string is unterminated */
untermstring    \"({character}|{escape})*{newline}

comment         [/][/].*
multlncomment   "/*"([^*]|\*+[^*/]|\n)*\*+\/
untermcomment   "/*"([^*]|\*+[^*/]|\n)*

identifier      {letter}({letter}|{integer})*   
illidentifier   {integer}+{letter}+

int yylineno = 1; 

%%

 /* rules */

 /* Keywords */
"if"      { updateCol(); return IF; }
"else"    { updateCol(); return ELSE; }
"while"   { updateCol(); return WHILE; }
"int"     { updateCol(); return INT; }
"char"    { updateCol(); return CHAR; }
"return"  { updateCol(); return RETURN; }
"void"    { updateCol(); return VOID; }

"+"       { updateCol(); return PLUS; }
"-"       { updateCol(); return MINUS; }
"*"       { updateCol(); return MULT; }
"/"       { updateCol(); return DIVIDE; }
"<"       { updateCol(); return LESSTHAN; }
">"       { updateCol(); return GREATERTHAN; }
"<="      { updateCol(); return LESSTHANEQ; }
">="      { updateCol(); return GREATERTHANEQ; }
"=="      { updateCol(); return EQ; }
"!="      { updateCol(); return NEQ; }
"="       { updateCol(); return ASSIGN; }
"["       { updateCol(); return LBRACKET; }
"]"       { updateCol(); return RBRACKET; }
"{"       { updateCol(); return LBRACE; }
"}"       { updateCol(); return RBRACE; }
"("       { updateCol(); return LPARENTHESIS; }
")"       { updateCol(); return RPARENTHESIS; }
","       { updateCol(); return COMMA; }
";"       { updateCol(); return SEMICOLON; }
"@"       { updateCol(); return OPER_AT; }
"++"      { updateCol(); return OPER_INC; }
"--"      { updateCol(); return OPER_DEC; }
"&&"      { updateCol(); return OPER_AND; }
"||"      { updateCol(); return OPER_OR; }
"!"       { updateCol(); return OPER_NOT; }

 /* Identifiers */
{identifier} {
    updateCol();
    yylval.strval = strdup(yytext);
    return ID;
}

{integer} {
    updateCol();
    yylval.value = atoi(yytext);
    return INTCONST;
}
{illidentifier}   { updateCol(); return ILLEGAL_TOK; }

 /* Constants */
{integerlead0}    { updateCol(); return ILLEGAL_TOK; }
{newline}         { countLines(); }
{whitespace}      { updateCol(); }
{charconst}       { updateCol(); yylval.value = yytext[1]; return CHARCONST;}
{string}          { return processString(); }
{untermstring} {
    int startCol  = scancol;

    scan_error_msg = "String spans multiple lines";
    for(int i = 0; yytext[i] != '\0';i++){
        if(yytext[i] == '\n') countLines();
        else scancol++;
    }
    yycol = startCol;
    /* error_location = token_start */
    return ERROR;
}
{untermstring_eof} {
    yycol = scancol;                 /* opening quote col */
    scan_error_msg = "Unterminated string constant";
    scancol += yyleng;
    return ERROR;
}

 /* Comments */
{comment}         { updateCol(); }
{multlncomment}   {
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') countLines();
        else scancol++;
    }
}
{untermcomment} {
    int startLine = yylineno;
    int startCol  = scancol;

    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') countLines();
        else scancol++;
    }

    yylineno = startLine;
    yycol    = startCol;
    scan_error_msg  = "Unterminated comment";
    return ERROR;
}

 /* Other */
.                 { updateCol(); return ILLEGAL_TOK;}

%%

/* user routines */

/* TODO:
Implement this function to keep track of column numbers
*/
void updateCol(){
   yycol = scancol;
   scancol += yyleng;
}

/* TODO:
Implement this function to keep track of line numbers
*/
void countLines(){
   yycol = scancol;
   scancol = 1;
   ++yylineno;
}


/* TODO:
Implement this function to
check for illegal escape sequences in string literals
and
convert valid escape sequences into escaped characters
(string[0] == '\\' and string[1] == 'n', then string[0] should be '\n')
*/
int processString() {
    int startCol = scancol;   /* column of opening " */
    int i = 1;                /* read index: skip opening quote */
    int out = 0;              /* write index: overwrite yytext with contents only */

    while (yytext[i] != '"' && yytext[i] != '\0') {
        if (yytext[i] == '\\') {
            int escCol = startCol + i + 1;   /* column of the '\' */

            i++;  /* move to escaped char */
            if (yytext[i] == '\0') {
                yycol = startCol;
                scan_error_msg = "Unterminated string constant";
                scancol += yyleng;
                return ERROR;
            }

            if (yytext[i] == 'n')      { yytext[out++] = '\n'; i++; }
            else if (yytext[i] == 't') { yytext[out++] = '\t'; i++; }
            else if (yytext[i] == '\\'){ yytext[out++] = '\\'; i++; }
            else if (yytext[i] == '"') { yytext[out++] = '"';  i++; }
            else {
                yycol = escCol;
                scan_error_msg = "Unrecognized escape character in String";
                scancol += yyleng;
                return ERROR;
            }
        } else {
            yytext[out++] = yytext[i++];
        }
    }

    if (yytext[i] != '"') {
        yycol = startCol;
        scan_error_msg = "Unterminated string constant";
        scancol += yyleng;
        return ERROR;
    }

    yytext[out] = '\0';  /* yytext = contents */
    yycol = startCol;    /* token start @ opening quote */
    scancol += yyleng;   /* advance past full original lex */
    return STRCONST;
}
