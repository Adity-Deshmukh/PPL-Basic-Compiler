/*
 *Group no: 8
 *Jai Doshi 2018A7PS0012P
 *Aditya Deshmukh 2018A7PS0246P
 *Shah Jainam Satishkumar 2018A7PS0212P
 *
 * */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "CreateParseTree.h"

void loadProductionRule (ProductionRuleNode * node, Stack * PDA)
{
    if(node==NULL)
		return;
	loadProductionRule(node->next, PDA);
	
	push(node->symbol, PDA);
}

void freeParseTree (ParseTree * t)
{
	/*for(int i=0; i<t->num_children; ++i)
		freeParseTree(t->children[i]);
	
	free(t);*/
}

int expand (ParseTree ** t, TokenStream ** ptr_current_token, int production_rule, Stack * PDA)
{
	if(production_rule!=-1)
	{
		//We are expanding a new production rule; create the node corresponding to it and load it on the stack
		*t = (ParseTree*) malloc(sizeof(ParseTree));
		strcpy((*t)->symbol, grammar_rules.arr[production_rule].nonterminal);
		strcpy((*t)->terminal, "");
		(*t)->grammar_rule = production_rule;
		(*t)->num_children = 0;
		
		
		loadProductionRule(grammar_rules.arr[production_rule].head, PDA);
	}
	
	
	while(*ptr_current_token)
	{
		if(PDA->size==0)
		{
			//deleteStack(PDA);
			return 1;
		}
		char * stack_top = top(PDA)->data;
		//printf("%d %s %s %s %s\n", PDA->size, grammar_rules.arr[production_rule].nonterminal, stack_top, (*ptr_current_token)->token_name, (*ptr_current_token)->lexeme);
		
		if(stack_top[0]-'A'>=0 && stack_top[0]-'A'<=25 && strcmp(stack_top, "R1"))
		{
			
		    //Top of stack is a non-terminal
			for(int i=0; i<grammar_rules.size; ++i)
			{
				if(!strcmp(stack_top, grammar_rules.arr[i].nonterminal))
				{
					//printf("%d\n", i);
					Stack * temp_stack = malloc(sizeof(Stack));
					temp_stack->size = 0;
					TokenStream ** temp_ptr_current_token = (TokenStream **) malloc(sizeof(TokenStream *));
					*temp_ptr_current_token = *ptr_current_token;
					int temp_children = (*t)->num_children;
					
					if(expand(&((*t)->children[(*t)->num_children]), temp_ptr_current_token, i, temp_stack))
					{
						//Trying a production rule	
						temp_stack = createStackCopy(PDA);
						pop(temp_stack);
						++((*t)->num_children);
						
						//printf("\nhere %d\n", (temp_stack)->size);
						//printf("%d %s %s %s\n", PDA->size, grammar_rules.arr[production_rule].nonterminal, stack_top, (*ptr_current_token)->token_name);
						//printf("\nhere %s", top(temp_stack)->data);
			
						if(expand(t, temp_ptr_current_token, -1, temp_stack))
						{
							//If we are able to use the production rule, check if we can empty the rest of the stack
						    *ptr_current_token = *temp_ptr_current_token;
							
							//free(temp_ptr_current_token);
							//deleteStack(PDA);
							return 1;
						}
						
					}	
					//free(temp_ptr_current_token);
					for(int j=temp_children; j<(*t)->num_children; ++j)
					{
						//Delete the created parse trees if we are not able to apply the production rule and revert
						freeParseTree((*t)->children[j]);
					}
					(*t)->num_children = temp_children;
				}
			}
			//deleteStack(PDA);
			return 0;
		}
		
		else if(!strcmp(stack_top, "epsilon"))
		{
			//Top of stack is epsilon
			(*t)->children[(*t)->num_children] = (ParseTree *) malloc(sizeof(ParseTree));
			(*t)->children[(*t)->num_children]->num_children = 0;
			strcpy((*t)->children[(*t)->num_children]->symbol, "epsilon");
			strcpy((*t)->children[(*t)->num_children]->terminal, "epsilon");
			
			++((*t)->num_children);
			
			pop(PDA);
			continue;
		}
		
		else if(strcmp(stack_top, (*ptr_current_token)->token_name))
		{
			//Top of stack does not match with token
			//deleteStack(PDA);
			return 0;
		}
		
		else
		{
			//Top of stack matches with token 
			pop(PDA);
			
			(*t)->children[(*t)->num_children] = (ParseTree *) malloc(sizeof(ParseTree));
			(*t)->children[(*t)->num_children]->num_children = 0;
			strcpy((*t)->children[(*t)->num_children]->symbol, (*ptr_current_token)->token_name);
			strcpy((*t)->children[(*t)->num_children]->terminal, (*ptr_current_token)->lexeme);
			
			(*t)->children[(*t)->num_children]->line_no = (*ptr_current_token)->line_no;
			
			++((*t)->num_children);
			*ptr_current_token = (*ptr_current_token)->next;
		}
	}
	//deleteStack(PDA);
	return 1;
}


ParseTree * createParseTree(ParseTree *t, TokenStream *s, Grammar G)
{

	Stack * PDA = (Stack*) malloc(sizeof(Stack)) ;
	PDA->size = 0;
	grammar_rules = G;
	
	for(int i=0; i<grammar_rules.size; ++i)
	{
		TokenStream ** ptr_token = (TokenStream **) malloc(sizeof(TokenStream*));
		*ptr_token = s;
		if(!strcmp("S", grammar_rules.arr[i].nonterminal))
		{
			if(!expand(&t, ptr_token, i, PDA))
			{
				printf("Error creating parse tree!");
				return t;
			}
		//	free(ptr_token);
		}
	}
	printf("parse tree is created successfully\n");
	return t;
}


void printNodeType(typeExpressionTable T)
{
	  
        printf("Field 1 (Lexeme Name) : %s | ",T->name);
        printf("Field 2 (Data Type) : ");
        switch(T->data_type){
            case 0:
                printf("Primitive");
                break;
            case 1:
                printf("Rectangular_Array");
                break;
            case 2:
                printf("Jagged_Array");
                break;
        }
        printf(" | Field 3 (Array Type) : ");
        switch(T->array_type){
            case 0:
                printf("Static Array");
                break;
            case 1:
                printf("Dynamic Array");
                break;
            case 2:
                printf("Not Applicable");
                break;
        }
        printf(" | Field 4 (Type Expression) : ");
        if(T->data_type==0){
            if(T->type_expression.prim.prim_type==0){
                printf("<type = Integer>");
            }
            if(T->type_expression.prim.prim_type==1){
                printf("<type = Real>");
            }
            if(T->type_expression.prim.prim_type==2){
                printf("<type = Bool>");
            }
        }
        else if(T->data_type==1){
            printf("<type = RectArray,");
            
            // if(T->array_type==0)
            //     printf(" Static");
            // else
            //     printf(" Dynamic");

            rectArray r = T->type_expression.rect; 
            rectRange* range = r.range;

            int n = r.dimension;
            printf(" dimensions = %d, ",n);

            for(int i=1;i<=n;i++){
                printf("range_R%d = (%s,%s), ",i,range->low,range->high);
                range=range->next;
            }

            printf("basicElementType = Integer");
        }
        else{
            printf("<type = JaggedArray,");
            
            printf(" dimensions = ");
            if(T->is2Djagged){
                printf("2, ");
                twoDimJagged j2d;
                j2d = T->type_expression.jagged_twod;
     
                printf("range_R1 = (%d,%d), ",j2d.R1_lo,j2d.R1_hi);
                printf("range_R2 = ");

                jagged2drange* range = j2d.dimension_list;
                printf("(");
                while(range){
                    printf("%d, ",range->dimension);
                    range = range->next;
                }
                printf(")");
            }
            else{
                printf("3, ");
                threeDimJagged j3d;
                j3d = T->type_expression.jagged_threed;
            
                printf("range_R1 = (%d,%d), ",j3d.R1_lo,j3d.R1_hi);
                printf("range_R2 = ");

                jagged3d_R2_range* r2 = j3d.R2_dimension_list;
                printf("(");
                while(r2){
                    printf("%d, ",r2->R3_size);
                    jagged3d_R3_range* r3 = r2->R3_dimension_list;
                    printf(" [ ");
                    while(r3){
                        printf("%d ",r3->dimension);
                        r3=r3->next;
                    }
                    printf(" ] ");
                    r2=r2->next;
                }
                printf(")");
            }

            printf("basicElementType = Integer");
        
        }
        
        T = T->next;
}

void printTree(ParseTree *t,int d){
    if(t==NULL)
    return;
    if(!strcmp(t->terminal,"")){
         printf("symbol:%s,",t->symbol);
        printf(" non terminal,");
	  printf("Type Expression:");  
	// printNodeType(&(t->typeExprT));
        int index=(t->grammar_rule);
        printf(", grammar rule:%s",grammar_rules.arr[index].nonterminal);
        ProductionRuleNode *temp=grammar_rules.arr[index].head;
        while(temp!=NULL){
            printf(" %s",temp->symbol);
            temp=temp->next;
        }
         printf(", depth:%d",d);
    }
    else{
        printf("symbol:%s,",t->symbol);
        printf(" terminal,");
        printf(" lexeme:%s,",t->terminal);
        if(!strcmp(t->symbol,"epsilon")){}
        else
        printf(" line_num:%d,",t->line_no);
        printf(" depth:%d",d);
    }
    printf("\n");
    printf("\n");
    for(int i=0;i<(t->num_children);i++){
    printTree(t->children[i],d+1);
    }
}
void printParseTree(ParseTree *t){
printTree(t,0);
}
