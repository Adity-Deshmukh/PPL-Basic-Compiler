/*
 *  *Group no: 8
 *   *Jai Doshi 2018A7PS0012P
 *    *Aditya Deshmukh 2018A7PS0246P
 *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *
 *       * */
#include<stdlib.h>
#include<string.h>
#include <stdio.h>
#include "ReadGrammar.h"


Grammar readGrammar(Grammar G){
G.arr=(ProductRuleLinkedlist *)malloc(sizeof(ProductRuleLinkedlist)*100);

//Assumed that the total number of production rules in grammar.txt file will be less than 100.

FILE *fptr=fopen("grammar.txt","r");
if(fptr == NULL){
    perror("Error opening file"); // print error
    return G;
  }
  
G.size = 0;

char linestr[1000];

int i=0;

while(fgets(linestr, sizeof(linestr),fptr)!=NULL){
	char *token=strtok(linestr," \n\t\r");
	if(token==NULL)
		continue;
	
	++G.size;
	strcpy(G.arr[i].nonterminal,token);
	ProductionRuleNode *temp=(ProductionRuleNode *)malloc(sizeof(ProductionRuleNode));
	token=strtok(NULL," \n\t\r");
	strcpy(temp->symbol,token);
	G.arr[i].head=temp;

	token=strtok(NULL," \n\t\r");
	while(token!=NULL){

		temp->next=(ProductionRuleNode *)malloc(sizeof(ProductionRuleNode));
		temp=temp->next;
		temp->next=NULL;
		strcpy(temp->symbol,token);
		token=strtok(NULL," \n\t\r");
	}
	++i;
}

fclose(fptr);

return G;
}


