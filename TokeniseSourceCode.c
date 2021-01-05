/*
 *  *Group no: 8
 *   *Jai Doshi 2018A7PS0012P
 *    *Aditya Deshmukh 2018A7PS0246P
 *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *
 *       * */
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "TokeniseSourceCode.h"

char *getTokenName(char *lex){
 if(!strcmp(lex,"program"))
        return "program";
    else if(!strcmp(lex,"("))
        return "br_op";
    else if(!strcmp(lex, ")"))
	    return "br_cl";
    else if(!strcmp(lex,"{"))
        return "cur_op";
    else if(!strcmp(lex,"}"))
        return "cur_cl";
    else if(!strcmp(lex,"declare"))
        return "declare";
    else if(!strcmp(lex,":"))
        return "colon";
    else if(!strcmp(lex,"list"))
        return "list";
    else if(!strcmp(lex,"of"))
        return "of";
    else if(!strcmp(lex,"variables"))
        return "variables";
    else if(!strcmp(lex,";"))
        return "semicolon";
    else if(!strcmp(lex,"integer"))
        return "integer";
    else if(!strcmp(lex,"boolean"))
        return "boolean";
    else if(!strcmp(lex,"real"))
        return "real";
    else if(!strcmp(lex,"array"))
        return "array";
    else if(!strcmp(lex,"jagged"))
        return "jagged";
    else if(!strcmp(lex,".."))
        return "dot_dot";
    else if(!strcmp(lex,"R1"))
        return "R1";
    else if(!strcmp(lex,"["))
        return "sq_op";
    else if(!strcmp(lex,"]"))
        return "sq_cl";
    else if(!strcmp(lex,"size"))
        return "size";
    else if(!strcmp(lex,"values"))
        return "values";
    else if(!strcmp(lex,"+"))
        return "add_op";
    else if(!strcmp(lex,"-"))
        return "sub_op";
    else if(!strcmp(lex,"*"))
        return "mul_op";
    else if(!strcmp(lex,"/"))
        return "div_op";
    else if(!strcmp(lex,"="))
        return "assign_op";
    else if(!strcmp(lex,"|||"))
        return "or_op";
    else if(!strcmp(lex,"&&&"))
        return "and_op";
    else if(lex[0]-'0' >= 0 && lex[0]-'0' <= 9) 
         return "integer_literal";
    else return "id";

}


TokenStream* tokeniseSourcecode(TokenStream *s){

TokenStream *head=s;
char linestr[128];
int linenum=0;
	while(!feof(fp)){
	linenum++;
	fgets(linestr,sizeof(linestr),fp);
	char *token;
	token=strtok(linestr," \t\n\r");
		while(token!=NULL){
		s->next=(TokenStream* )malloc(sizeof(TokenStream));
		s=s->next;
		s->next=NULL;
		strcpy(s->lexeme,token);
		s->line_no=linenum;
		strcpy(s->token_name,getTokenName(token));
		token=strtok(NULL," \t\n\r");
		}
	}
fclose(fp);
TokenStream* temp = head->next;
free(head);
return temp;
}
