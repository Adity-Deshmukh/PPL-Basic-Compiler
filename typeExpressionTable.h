/*
 *  *Group no: 8
 *   *Jai Doshi 2018A7PS0012P
 *    *Aditya Deshmukh 2018A7PS0246P
 *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *
 *       * */
#include "CreateParseTree.h"
#include <string.h>
#include <ctype.h>

typedef struct typeExprTable* typeExpressionTable;

typeExpressionTable start;
typeExpressionTable head;

void traverseParseTree(ParseTree *t, typeExpressionTable T);

void generate_type_expr_table(ParseTree *t, typeExpressionTable T, int depth);

int generate_assign_type(ParseTree *t, typeExpressionTable T, int depth);

void generate_decl_type(ParseTree *t, typeExpressionTable T, int depth);


void processSinglePrim(ParseTree *t, typeExpressionTable T, int depth);

void processMultPrim(ParseTree *t, typeExpressionTable T, int depth);

void processMultJagged(ParseTree *t, typeExpressionTable T,int depth);

void processMultRect(ParseTree *t, typeExpressionTable T, int depth);

void processSingleJagged(ParseTree *t, typeExpressionTable T, int depth);

void processSingleRect(ParseTree *t, typeExpressionTable T, int depth);

void printTypeExpression (typeExpressionTable  T);

void printTypeExpressionTable (typeExpressionTable T);

int checkDeclared(typeExpressionTable T,ParseTree* t,char name[500], int depth);

//add assumptions
//the var name length should be less than 100 chars
//arrType can have only 3 vals
//size of rect array is less than 100 aur if dynamic then length of identifier is less than = 2
// assuming max of 10 vars can be declared in a single decl statement

// page 3 and page 7 last lines
// undeclared vars 
