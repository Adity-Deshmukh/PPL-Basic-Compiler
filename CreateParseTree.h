/*
 *  *Group no: 8
 *   *Jai Doshi 2018A7PS0012P
 *    *Aditya Deshmukh 2018A7PS0246P
 *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *
 *       * */
#include "stack.h"
#include "ReadGrammar.h"
#include "TokeniseSourceCode.h"

//////////////////////////

Grammar grammar_rules;

typedef enum dataType{PRIMITIVE,RECT_ARRAY,JAGGED_ARRAY} dataType;

typedef enum arrayType{STATIC,DYNAMIC,NOT_APPLICABLE} arrayType;

typedef enum primitiveType{INT,REAL,BOOL} primitiveType;

// primitive type struct
typedef struct primitive{
    primitiveType prim_type;
}primitive;

// jagged array type struct
// 2D
typedef struct jagged2drange{
    int dimension;
	struct jagged2drange * next;
}jagged2drange;

typedef struct twoDimJagged{
    primitiveType prim_type;
	int R1_lo;
    int R1_hi;
	jagged2drange * dimension_list;
}twoDimJagged;


// 3D
typedef struct jagged3d_R3_range{
	int dimension;
    struct jagged3d_R3_range * next;
}jagged3d_R3_range;

typedef struct jagged3d_R2_range{
	struct jagged3d_R2_range * next;
	int R3_size;
    jagged3d_R3_range * R3_dimension_list;
}jagged3d_R2_range;

typedef struct threeDimJagged{
    primitiveType prim_type;
	int R1_lo;
	int R1_hi;
    jagged3d_R2_range * R2_dimension_list;
}threeDimJagged;

// rect array type struct
typedef struct rectRange{
    char low[50]; 
    char high[50];
    struct rectRange* next;
}rectRange;

typedef struct rectArray{
    int dimension;
    rectRange* range;
}rectArray;



// type union
typedef union typeExpr{
    primitive prim;
    twoDimJagged jagged_twod;
    threeDimJagged jagged_threed;
    rectArray rect;
}typeExpr;


typedef struct typeExprTable{
    char name[500];
    dataType data_type;
    arrayType array_type;
    typeExpr type_expression;
    int is2Djagged; //1 if 2D jagged    //0 if 3D   
    struct typeExprTable* next;
}typeExprTable;

typedef struct typeExprTable* typeExpressionTable;
//////////////////////////

typedef struct ParseTree
{
	int num_children;
	char symbol[50];
	char terminal[50];
	int line_no;
	int grammar_rule;
	typeExprTable typeExprT;
	struct ParseTree * children [20]; 
}ParseTree;

void printNodeType(typeExpressionTable T);

void loadProductionRule (ProductionRuleNode * node, Stack * PDA);

void freeParseTree (ParseTree * t);

int expand (ParseTree ** t, TokenStream ** current_token, int production_rule, Stack * PDA);

ParseTree * createParseTree(ParseTree *t, TokenStream *s, Grammar G);

void printTree(ParseTree *t,int d);
void printParseTree(ParseTree *t);
