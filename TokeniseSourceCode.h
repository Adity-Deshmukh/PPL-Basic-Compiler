/*
 *  *Group no: 8
 *   *Jai Doshi 2018A7PS0012P
 *    *Aditya Deshmukh 2018A7PS0246P
 *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *
 *       * */

#include <stdio.h>

typedef struct TokenStream{
char token_name[20];
char lexeme[50];
int line_no;
struct TokenStream *next;
} TokenStream;

FILE *fp;

TokenStream* tokeniseSourcecode(TokenStream *s);
char *getTokenName(char *lex);
