/*
 *  *  *  *Group no: 8
 *   *   *   *Jai Doshi 2018A7PS0012P
 *    *    *    *Aditya Deshmukh 2018A7PS0246P
 *     *     *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *      *      *
 *       *       *       * */

#include "typeExpressionTable.h"

// head = malloc(sizeof(struct typeExprTable));
typeExpressionTable start;

int checkDeclared(typeExpressionTable T,ParseTree* t,char name[500], int depth){	
    // printf("Inside check declared\n");	
    if(T==NULL)	
        return 0;	
    T = T->next;	
    while(T){	
        if(strcmp(T->name,name)==0){	
		    printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: Variable \" %s \" already declared\n\n", depth, name);	
            return 1;	
        }	
        T = T->next;	
    }	
    return 0;	
    // 	printf("Line number: %d, operator: %s ", line_no, operator);	
	// printTypeExpression (&type1);	
	// printTypeExpression (&type2);	
	// printf("depth: %d, Error: %s\n\n", depth, message);	
}

void traverseParseTree(ParseTree *t, typeExpressionTable T){
    /*
        This function is to process the main body only
        S program brackets cur_op Body cur_cl
    */
    if(t)
    {   
       // printf("\n\n");
       int n = t->num_children;

       for(int i=0;i<n;i++)
       {
           if(strcmp(t->children[i]->symbol ,"Body")==0){
              // printf("Inside Body\n");
               generate_type_expr_table(t->children[i],T, 2);
           }
       }
    }
}

void generate_type_expr_table(ParseTree *t, typeExpressionTable T, int depth){
    /*
        This function is to recurse 
        Statements Statement Statements
        Statement Declaration_statement 
        Statement Assignment_statement

        split into assign and decl
    */
    if(t){
        int n = t->num_children;

        for(int i=0;i<n;i++){
            if(strcmp(t->children[i]->symbol,"Statement")==0){
               // printf("Inside Statement\n");
                ParseTree* child = t->children[i];
                if(strcmp(child->children[0]->symbol,"Declaration_statement")==0){
                  //  printf("Inside Decl Statement\n");
                    generate_decl_type(child->children[0],T, depth+1);
                }
                else{
                     generate_assign_type(child->children[0],T, depth+1);
                }
            }
            else{
                generate_type_expr_table(t->children[i],T, depth+1);
            }
        }
    }
}


void generate_decl_type(ParseTree *t, typeExpressionTable T, int depth){
    /*
        Declaration_statement Declare_single
        Declaration_statement Declare_multiple
        Declare_single Declare_single_array
        Declare_single Declare_single_primitive
        Declare_multiple Declare_multiple_primitive
        Declare_multiple Declare_multiple_array

        Declare_multiple_array Declare_multiple_rect_array 
        Declare_multiple_array Declare_multiple_jagged_array
        Declare_single_array Declare_single_rect_array
        Declare_single_array Declare_single_jagged_array        
    */
    if(t){
        // printf("symbol -> %s\n",t->symbol);

        ParseTree* child = t->children[0];
        if(strcmp(child->symbol,"Declare_single")==0){                
           // printf("Inside Decl Single Statement\n");

            if(strcmp(child->children[0]->symbol,"Declare_single_array")==0){
                ParseTree* subChild = child->children[0];
                
               // printf("Inside Decl Single array Statement\n");
                if(strcmp(subChild->children[0]->symbol,"Declare_single_rect_array")==0){

                    // printf("Inside Decl Single rect array Statement\n");
                    // printf("\nsymbol -> %s  || %d\n",subChild->children[0]->symbol,child->children[0]->num_children);
                    processSingleRect(subChild->children[0],T, depth+1);
                }
                else{
                    processSingleJagged(subChild->children[0],T, depth+1);
                }

            }
            else{
                // printf("Inside Decl Single primitive Statement\n");
                
                processSinglePrim(child->children[0],T, depth+1);
            }


        }
        if(strcmp(child->symbol,"Declare_multiple")==0){
         //   printf("Inside Decl Multiple Statement\n");

            // printf("symbol -> %s  || %s\n",child->symbol,child->children[0]->symbol);

            if(strcmp(child->children[0]->symbol,"Declare_multiple_array")==0){
                ParseTree* subChild = child->children[0];
             //   printf("Inside Decl Multiple array Statement\n");
                       
                if(strcmp(subChild->children[0]->symbol,"Declare_multiple_rect_array")==0){
                    // printf("Inside Decl Multiple rect array Statement\n");
                    // printf("\nsymbol -> %s  || %d\n",subChild->children[0]->symbol,subChild->children[0]->num_children);
                    processMultRect(subChild->children[0],T, depth+1);
                }
                else{
              //      printf("Inside Decl Multiple jagged array Statement\n");

                    processMultJagged(subChild->children[0],T,depth+1);
                }

            }
            else{
                // printf("Inside Decl Multiple prim Statement\n");
                // printf("symbol -> %s  || %d\n",child->children[0]->symbol,child->children[0]->num_children);
                processMultPrim(child->children[0],T, depth+1);
            }
        }
    }
}


void processSinglePrim(ParseTree *t, typeExpressionTable T, int depth){
    /*
        Declare_single_primitive declare id colon type semicolon
    */

        // printf("symbol : %s || %d",t->symbol,t->num_children);
        
        typeExpressionTable item = malloc(sizeof(struct typeExprTable));
		   ParseTree* idNode;
        
        int n=t->num_children;
        // printf("n = %d",n);
        for(int i=0;i<n;i++){
            // printf("symbol : %s || %s \n",t->children[i]->symbol,t->children[i]->terminal);
            if(strcmp(t->children[i]->symbol,"id")==0){
                // getname here
				 idNode = t->children[i];
                strcpy(item->name,t->children[i]->terminal);
            }
            // check case for type
            else if(strcmp(t->children[i]->symbol,"Type")==0){
                // printf("\nGetting type");
                item->data_type = 0;
                item->array_type = 2;
                ParseTree* child = t->children[i];
                // printf("symbol : %s || %s \n",child->children[0]->symbol,child->children[0]->terminal);
                if(strcmp(child->children[0]->symbol,"integer")==0){
                    item->type_expression.prim.prim_type = 0;
                }
                else if(strcmp(child->children[0]->symbol,"real")==0){
                    item->type_expression.prim.prim_type = 1;
                }
                else{
                    item->type_expression.prim.prim_type = 2;
                }
            }
        }
        // printf("\n\n");
		
				
        if(checkDeclared(start,idNode,item->name, depth)){	
            free(item);	
            return ;	
        }

        t->typeExprT.data_type = item->data_type;
        t->typeExprT.array_type = item->data_type;
        t->typeExprT.type_expression = item->type_expression;

        // T->next = item;
        // T = T->next;
        head->next = item;
        head = head->next;

        if(start==NULL){
            start = malloc(sizeof(struct typeExprTable)); 
            start->next = head;
        }
        // printTypeExpressionTable(T);

        // printf("\nGenerated single prim type");
    
}


void getVariableList(ParseTree *t,char varList[10][50],int* count){
    // printf("\nInside variable list");
    // printf("\nsymbol : %s || %s     %d",t->symbol,t->terminal,t->num_children);

    if(t->num_children==1)
        return ;
    else{
        // printf("\nGenerated var %s %d",t->children[0]->terminal,*count);
        strcpy(varList[(*count)++],t->children[0]->terminal);
        getVariableList(t->children[1],varList,count);        
    }

    // if(strcmp(t->symbol,"epsilon")==0){
    //     return ;
    // }
    // else{        
    //     strcpy(varList[(*count)++],t->children[0]->terminal);
    //     getVariableList(t->children[1],varList,count);
    // }
}

void processMultPrim(ParseTree *t, typeExpressionTable T, int depth){
    if(t){

        struct typeExprTable item;
        char varList[10][50];
        int *count;
        int zero=0;
        count=&zero;
        int n=t->num_children;
		      ParseTree* idNode;
        // printf("symbol : %s || %s",t->symbol,t->terminal);

        for(int i=0;i<n;i++){
            if(strcmp(t->children[i]->symbol,"id")==0){
                // getname here
                // printf("\nGenerated var %s %d",t->children[i]->terminal,*count);
				idNode = t->children[i];
                strcpy(varList[(*count)++],t->children[i]->terminal);
            }
            else if(strcmp(t->children[i]->symbol,"Variables")==0){
                //get variable list
                // printf("%d",t->children[i]->num_children);
                getVariableList(t->children[i],varList,count) ;
            }          
            // check case for type
            else if(strcmp(t->children[i]->symbol,"Type")==0){
                item.data_type = 0;
                item.array_type = 2;
                ParseTree* child = t->children[i];
                if(strcmp(child->children[0]->symbol,"integer")==0){
                    item.type_expression.prim.prim_type = 0;
                }
                else if(strcmp(child->children[0]->symbol,"real")==0){
                    item.type_expression.prim.prim_type = 1;
                }
                else{
                    item.type_expression.prim.prim_type = 2;
                }
            }
        }


        
        for(int i=0;i<*count;i++){
            // printf("\n%s",varList[i]);
            typeExpressionTable item_i = malloc(sizeof(struct typeExprTable));
            strcpy(item_i->name,varList[i]);
			      if(checkDeclared(start,idNode,item_i->name, depth)){	
                free(item_i);	
                continue ;	
            }
            item_i->data_type = item.data_type;
            item_i->array_type = item.array_type;
            item_i->type_expression.prim = item.type_expression.prim;
            head->next = item_i;
            head = head->next;
            // T->next = item_i;
            // T = T->next;
            if(start==NULL){
                start = malloc(sizeof(struct typeExprTable)); 
                start->next = head;
            }
            // printTypeExpressionTable(T);
        }

        t->typeExprT.data_type = item.data_type;
        t->typeExprT.array_type = item.data_type;
        t->typeExprT.type_expression = item.type_expression;
    }
}


int checkStatic(rectRange* r){

    // returns 1 if static else 0
    for(int i=0;i<50;i++){
        if(r->low[i]=='\0')
            break;
        if(!isdigit(r->low[i]))
            return 0;
    }
    for(int i=0;i<50;i++){
        if(r->high[i]=='\0')
            break;
        if(!isdigit(r->high[i]))
            return 0;
    }
    return 1;
}


void getRectDim(ParseTree *t,int* isStatic,rectRange* range,rectArray* rectArr){
    // printf("Inside get rect dim");
    // printf("symbol -> %s  || %d\n",t->symbol,t->num_children);
                
    if(t){
        if(t->num_children==1)
            return ;
        else{
            rectArr->dimension++;

            rectRange* r=malloc(sizeof(rectRange));
            strcpy(r->low,t->children[1]->children[0]->terminal);
            strcpy(r->high,t->children[3]->children[0]->terminal);
            // printf("(%s     %s)\n",r->low,r->high);
            range->next=r;
            range=range->next;

            *isStatic = (*isStatic) && checkStatic(r);

            ParseTree* ot_dim = t->children[5];

            getRectDim(ot_dim,isStatic,r,rectArr);
        }
    }
}


void processSingleRect(ParseTree *t, typeExpressionTable T, int depth){

    if(t){

        typeExpressionTable item=malloc(sizeof(struct typeExprTable));
        // printf("symbol -> %s  || %d\n",t->symbol,t->num_children);
		    ParseTree* idNode;
        int n = t->num_children;

        for(int i=0;i<n;i++){
            if(strcmp(t->children[i]->symbol,"id")==0){
                // getname here
				idNode= t->children[i];
                strcpy(item->name,t->children[i]->terminal);
                // printf("%s",item->name);
            }   
            else if(strcmp(t->children[i]->symbol,"Type_rect")==0){
                //get type rect here
                item->data_type = 1;
                int* isStatic;
                int one = 1;
                isStatic = &one;
            //     // get static/dynamic
            //     // get dimensions
                rectArray* rectArr = malloc(sizeof(rectArray));
                rectArr->dimension = 1;
                rectRange* r=malloc(sizeof(rectRange));
            //     // decl -> type_rect -> item ->
                strcpy(r->low,t->children[i]->children[2]->children[0]->terminal);
                strcpy(r->high,t->children[i]->children[4]->children[0]->terminal);

                // printf("(%s     %s)\n",r->low,r->high);
                *isStatic = checkStatic(r);

                ParseTree* ot_dim = t->children[i]->children[6];
                // printf("symbol -> %s  || %d\n",ot_dim->symbol,ot_dim->num_children);
                getRectDim(ot_dim,isStatic,r,rectArr);
				
				
                
                item->array_type = !(*isStatic);
                
                item->type_expression.rect.range = r;
                item->type_expression.rect.dimension = rectArr->dimension;
                

            }

        }
		
			        if(checkDeclared(start,idNode,item->name, depth)){	
            free(item);	
            return ;	
        }

        t->typeExprT.data_type = item->data_type;
        t->typeExprT.array_type = item->data_type;
        t->typeExprT.type_expression = item->type_expression;

        // T->next = item;
        // T = T->next;
        head->next = item;
        head = head->next;

        if(start==NULL){
            start = malloc(sizeof(struct typeExprTable)); 
            start->next = head;
        }

       // printTypeExpressionTable(T);
    }
}


void processMultRect(ParseTree *t, typeExpressionTable T, int depth){
    if(t){

        struct typeExprTable item;
        char varList[10][50];
        int *count;
        int zero=0;
        count=&zero;
        int n = t->num_children;
		       ParseTree* idNode;

        // printf("symbol : %s || %s",t->symbol,t->terminal);

        for(int i=0;i<n;i++){
            if(strcmp(t->children[i]->symbol,"id")==0){
                // getname here
                // printf("\nGenerated var %s %d",t->children[i]->terminal,*count);               
                strcpy(varList[(*count)++],t->children[i]->terminal);
				 idNode = t->children[i];
            }
            else if(strcmp(t->children[i]->symbol,"Variables")==0){
                //get variable list
                // printf("\nGenerated var %s %d",t->children[i]->symbol,*count);                
                getVariableList(t->children[i],varList,count) ;
            }
            else if(strcmp(t->children[i]->symbol,"Type_rect")==0){
            //     //get type rect here
                item.data_type = 1;
                int* isStatic;
                int one = 1;
                isStatic = &one;
            //     // get static/dynamic
            //     // get dimensions
                rectArray* rectArr = malloc(sizeof(rectArray));
                rectArr->dimension = 1;
                rectRange* r=malloc(sizeof(rectRange));

                strcpy(r->low,t->children[i]->children[2]->children[0]->terminal);
                strcpy(r->high,t->children[i]->children[4]->children[0]->terminal);
                // printf("(%s     %s)\n",r->low,r->high);
                *isStatic = checkStatic(r);

                ParseTree* ot_dim = t->children[i]->children[6];

                getRectDim(ot_dim,isStatic,r,rectArr);
                
                // printf("\nis static => %d\n",*isStatic);
                
                item.array_type = !(*isStatic);
                
                item.type_expression.rect.range = r;
                item.type_expression.rect.dimension = rectArr->dimension;
            }

        }

        for(int i=0;i<*count;i++){
            // printf("\n%s",varList[i]);
            typeExpressionTable item_i = malloc(sizeof(struct typeExprTable));
            strcpy(item_i->name,varList[i]);
			          if(checkDeclared(start,idNode,item_i->name, depth)){	
                free(item_i);	
                continue ;	
            }
            item_i->data_type = item.data_type;
            item_i->array_type = item.array_type;
            item_i->type_expression.rect = item.type_expression.rect;
            // T->next = item_i;
            // T = T->next;
            head->next = item_i;
            head = head->next;
            if(start==NULL){
                start = malloc(sizeof(struct typeExprTable)); 
                start->next = head;
            }
            //printTypeExpressionTable(T);
        }


        t->typeExprT.data_type = item.data_type;
        t->typeExprT.array_type = item.data_type;
        t->typeExprT.type_expression = item.type_expression;
    }
}

int processIntegerList (ParseTree *t)
{
	int num = 0;
	
	while(t->num_children==2)
	{
		++num;
		t = t->children[1];
	}
	
	return num;
}

void processSingle2DJagged (ParseTree *t, typeExpressionTable T, int depth)
{       
        typeExpressionTable item = malloc(sizeof(struct typeExprTable));
		
			ParseTree* idNode;	
        idNode = t->children[1];
		
		strcpy(item->name, t->children[1]->terminal);
      
        item->data_type = 2;
        item->array_type = 2;
		item->is2Djagged = 1;
		
		item->type_expression.jagged_twod.prim_type = 0;
		item->type_expression.jagged_twod.R1_lo = atoi(t->children[5]->children[1]->terminal);
		item->type_expression.jagged_twod.R1_hi = atoi(t->children[5]->children[3]->terminal);
	    item->type_expression.jagged_twod.dimension_list = (jagged2drange *) malloc(sizeof(jagged2drange));
		
		

		jagged2drange * ptr = item->type_expression.jagged_twod.dimension_list;
		
		t = t->children[9];
		
		int total_size = 0;
		
		ParseTree* temp = t->children[10];
		
		while(temp->num_children==3)
		{
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
			temp = temp->children[2];
		}
		
			int x = processIntegerList(t->children[11]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
		
		
		
		ptr->dimension = atoi(t->children[6]->terminal);
		
		if(total_size!=ptr->dimension)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
			return;
		}
		
		ptr->next = NULL;
		
		t = t->children[13];
		
		
		while(strcmp(t->children[0]->symbol, "epsilon"))
		{
			t=t->children[0];
			ptr->next = (jagged2drange *) malloc(sizeof(jagged2drange));
			ptr = ptr->next;
			int total_size = 0;
		
		ParseTree* temp = t->children[10];
		
		while(temp->num_children==3)
		{
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
			temp = temp->children[2];
		}
		
		int x = processIntegerList(t->children[11]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
		
		
		
		ptr->dimension = atoi(t->children[6]->terminal);
		
		if(total_size!=ptr->dimension)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
			return;
		}
			ptr->next = NULL;
			t=t->children[13];
		}

		
	    t->typeExprT.data_type = item->data_type;
        t->typeExprT.array_type = item->data_type;
        t->typeExprT.type_expression = item->type_expression;
		t->typeExprT.is2Djagged = item->is2Djagged;

        // T->next = item;
        // T = T->next;
		     if(checkDeclared(start,idNode,item->name, depth)){	
            free(item);	
            return ;	
        }
		
        head->next = item;
        head = head->next;

        if(start==NULL){
            start = malloc(sizeof(struct typeExprTable)); 
            start->next = head;
        }

        //printTypeExpressionTable(T);
}

void processMult2DJagged(ParseTree *t, typeExpressionTable T, int depth)	
{	
    // printf("symbol : %s || %d \n\n",t->symbol,t->num_children);	
    struct typeExprTable item; //= malloc(sizeof(struct typeExprTable));	
    ParseTree* idNode;	
    char varList[10][50];	
    int *count;	
    int zero=0;	
    count=&zero;	
    item.data_type = 2;	
    item.array_type = 2;	
    item.is2Djagged = 1;	
    item.type_expression.jagged_twod.R1_lo = atoi(t->children[10]->children[1]->terminal);	
    item.type_expression.jagged_twod.R1_hi = atoi(t->children[10]->children[3]->terminal);	
    item.type_expression.jagged_twod.dimension_list = (jagged2drange *) malloc(sizeof(jagged2drange));	
    	
    jagged2drange * ptr = item.type_expression.jagged_twod.dimension_list;	
    	
    int n = t->num_children;	
    for(int i=0;i<n;i++){	
        // printf(" %s ",t->children[i]->terminal);	
        if(strcmp(t->children[i]->symbol,"id")==0){	
            // getname here	
            // printf("\nGenerated var %s %d",t->children[i]->terminal,*count);               	
            idNode = t->children[i];	
            strcpy(varList[(*count)++],t->children[i]->terminal);	
        }	
        else if(strcmp(t->children[i]->symbol,"Variables")==0){	
            //get variable list	
            // printf("\nGenerated var %s %d",t->children[i]->symbol,*count);                	
            getVariableList(t->children[i],varList,count) ;	
        }	
    }	
     	
    t = t->children[14];	
    	
   int total_size = 0;
		
		ParseTree* temp = t->children[10];
		
		while(temp->num_children==3)
		{
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
			temp = temp->children[2];
		}
		
			int x = processIntegerList(t->children[11]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
		
		
		
		ptr->dimension = atoi(t->children[6]->terminal);
		
		if(total_size!=ptr->dimension)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
			return;
		}
		
		ptr->next = NULL;
		
		t = t->children[13];
		
		
		while(strcmp(t->children[0]->symbol, "epsilon"))
		{
			t=t->children[0];
			ptr->next = (jagged2drange *) malloc(sizeof(jagged2drange));
			ptr = ptr->next;
			int total_size = 0;
		
		ParseTree* temp = t->children[10];
		
		while(temp->num_children==3)
		{
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
			temp = temp->children[2];
		}
		
		int x = processIntegerList(t->children[11]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			else if(x!=1)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Value list too long");
				return;
			}
			total_size += x;
		
		
		
		ptr->dimension = atoi(t->children[6]->terminal);
		
		if(total_size!=ptr->dimension)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
			return;
		}
			ptr->next = NULL;
			t=t->children[13];
		}
    for(int i=0;i<*count;i++){	
        // printf("\n%s",varList[i]);	
        typeExpressionTable item_i = malloc(sizeof(struct typeExprTable));	
        strcpy(item_i->name,varList[i]);	
        	
        if(checkDeclared(start,idNode,item_i->name, depth)){	
            free(item_i);	
            continue ;	
        }	
        	
        item_i->data_type = item.data_type;	
        item_i->array_type = item.array_type;	
        item_i->type_expression = item.type_expression;	
        item_i->is2Djagged = item.is2Djagged;	
        // T->next = item_i;	
        // T = T->next;	
        head->next = item_i;	
        head = head->next;	
        if(start==NULL){	
            start = malloc(sizeof(struct typeExprTable)); 	
            start->next = head;	
        }	
        // printTypeExpressionTable(item_i);	
    }	
    	
    t->typeExprT.data_type = item.data_type;	
    t->typeExprT.array_type = item.data_type;	
    t->typeExprT.type_expression = item.type_expression;	
    t->typeExprT.is2Djagged = item.is2Djagged;	
    // // // T->next = item;	
    // // // T = T->next;	
    // // head->next = item;	
    // // head = head->next;	
    // // if(start==NULL){	
    // //     start = malloc(sizeof(struct typeExprTable)); 	
    // //     start->next = head;	
    // // }	
    // printTypeExpressionTable(T);	
}	


void processSingle3DJagged (ParseTree *t, typeExpressionTable T, int depth)
{
	    typeExpressionTable item = malloc(sizeof(struct typeExprTable));
		
		ParseTree* idNode;	
        idNode = t->children[1];
		
		strcpy(item->name, t->children[1]->terminal);
      
        item->data_type = 2;
        item->array_type = 2;
		item->is2Djagged = 0;
		
		item->type_expression.jagged_threed.prim_type = 0;
		item->type_expression.jagged_threed.R1_lo = atoi(t->children[5]->children[1]->terminal);
		item->type_expression.jagged_threed.R1_hi = atoi(t->children[5]->children[3]->terminal);
	    item->type_expression.jagged_threed.R2_dimension_list = (jagged3d_R2_range *) malloc(sizeof(jagged3d_R2_range));
		
		jagged3d_R2_range * ptr = item->type_expression.jagged_threed.R2_dimension_list;
		
		t = t->children[9];
		
		ptr->R3_size = atoi(t->children[6]->terminal);
		
		ptr->R3_dimension_list = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
	    jagged3d_R3_range * ptr2 = ptr->R3_dimension_list;
		ptr2->next = NULL;
		ptr->next = NULL;
		
		ParseTree* temp = t->children[10];
		
		int count = 0;
		
		if(temp->num_children==3)
		{
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,", t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			ptr2->dimension=x;
			temp = temp->children[2];
			count++;
		}
	
		while(temp->num_children==3)
		{
			ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
			ptr2 = ptr2->next;
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			ptr2->dimension = x;
			ptr2->next=NULL;
			temp = temp->children[2];
			count++;
		}
		
		ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
		ptr2 = ptr2->next;
		int x = processIntegerList(t->children[11]);
		if(x==0)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
			return;
		}
		count++;
		
		
		if(ptr->R3_size != count)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
			return;
		}
		
		ptr2->dimension = x;
		ptr2->next=NULL;
		
		t = t->children[13];
		
		
		while(strcmp(t->children[0]->symbol, "epsilon"))
		{
			t=t->children[0];
			ptr->next = (jagged3d_R2_range *) malloc(sizeof(jagged3d_R2_range));
			ptr = ptr->next;
			ptr->R3_dimension_list = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
			jagged3d_R3_range * ptr2 = ptr->R3_dimension_list;
			ptr2->next = NULL;
		
			ParseTree* temp = t->children[10];
		
			int count = 0;
		
			if(temp->num_children==3)
			{
				int x = processIntegerList(temp->children[0]);
				if(x==0)
				{
						printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
					printf(" lexeme and type of second operator: ***, ");
					printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
					return;
				}
				ptr2->dimension=x;
				temp = temp->children[2];
				count++;
			}
	
			while(temp->num_children==3)
			{
				ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
				ptr2 = ptr2->next;
				int x = processIntegerList(temp->children[0]);
				if(x==0)
				{
						printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
				}
				ptr2->dimension = x;
				ptr2->next=NULL;
				temp = temp->children[2];
				count++;
			}
		
			ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
			ptr2 = ptr2->next;
			int x = processIntegerList(t->children[11]);
			if(x==0)
			{
					printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			count++;
			ptr2->dimension = x;
			ptr2->next=NULL;
			
			ptr->R3_size = atoi(t->children[6]->terminal);
			if(ptr->R3_size != count)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
				return;
			}
			ptr->next = NULL;
			t=t->children[13];
		}
		
			if(checkDeclared(start,idNode,item->name, depth)){	
            free(item);	
            return ;	
        }
		 
		
	    t->typeExprT.data_type = item->data_type;
        t->typeExprT.array_type = item->data_type;
        t->typeExprT.type_expression = item->type_expression;
		t->typeExprT.is2Djagged = item->is2Djagged;

        // T->next = item;
        // T = T->next;
        head->next = item;
        head = head->next;

        if(start==NULL){
            start = malloc(sizeof(struct typeExprTable)); 
            start->next = head;
        }

        //printTypeExpressionTable(T);

}

void processMult3DJagged(ParseTree *t, typeExpressionTable T,int depth){
    // typeExpressionTable item = malloc(sizeof(struct typeExprTable));
		struct typeExprTable item;
		// strcpy(item->name, t->children[1]->terminal);
        // printf("\nInside Mult3d jagged");
        char varList[10][50];
        int *count;
        int zero=0;
        count=&zero;
        ParseTree* idNode;
        item.data_type = 2;
        item.array_type = 2;
		item.is2Djagged = 0;
		
		item.type_expression.jagged_threed.prim_type = 0;
		item.type_expression.jagged_threed.R1_lo = atoi(t->children[10]->children[1]->terminal);
		item.type_expression.jagged_threed.R1_hi = atoi(t->children[10]->children[3]->terminal);
	    item.type_expression.jagged_threed.R2_dimension_list = (jagged3d_R2_range *) malloc(sizeof(jagged3d_R2_range));
		
		jagged3d_R2_range * ptr = item.type_expression.jagged_threed.R2_dimension_list;
		
        int n = t->num_children;

        for(int i=0;i<n;i++){
        // printf(" %s ",t->children[i]->terminal);
        if(strcmp(t->children[i]->symbol,"id")==0){
            // getname here
            // printf("\nGenerated var %s %d",t->children[i]->terminal,*count);               
            idNode = t->children[i];
            strcpy(varList[(*count)++],t->children[i]->terminal);
        }
        else if(strcmp(t->children[i]->symbol,"Variables")==0){
            //get variable list
            // printf("\nGenerated var %s %d",t->children[i]->symbol,*count);                
            getVariableList(t->children[i],varList,count) ;
        }
    }

    // printf("%d",*count);
        
		t = t->children[14];
		
		ptr->R3_size = atoi(t->children[6]->terminal);
		
		ptr->R3_dimension_list = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
	    jagged3d_R3_range * ptr2 = ptr->R3_dimension_list;
		ptr2->next = NULL;
		ptr->next = NULL;
		
		ParseTree* temp = t->children[10];
		
		int num = 0;
		
		if(temp->num_children==3)
		{
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,", t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			ptr2->dimension=x;
			temp = temp->children[2];
			num++;
		}
	
		while(temp->num_children==3)
		{
			ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
			ptr2 = ptr2->next;
			int x = processIntegerList(temp->children[0]);
			if(x==0)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			ptr2->dimension = x;
			ptr2->next=NULL;
			temp = temp->children[2];
			num++;
		}
		
		ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
		ptr2 = ptr2->next;
		int x = processIntegerList(t->children[11]);
		if(x==0)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
			return;
		}
		num++;
		
		
		if(ptr->R3_size != num)
		{
			printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
			printf(" lexeme and type of second operator: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
			return;
		}
		
		ptr2->dimension = x;
		ptr2->next=NULL;
		
		t = t->children[13];
		
		
		while(strcmp(t->children[0]->symbol, "epsilon"))
		{
			t=t->children[0];
			ptr->next = (jagged3d_R2_range *) malloc(sizeof(jagged3d_R2_range));
			ptr = ptr->next;
			ptr->R3_dimension_list = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
			jagged3d_R3_range * ptr2 = ptr->R3_dimension_list;
			ptr2->next = NULL;
		
			ParseTree* temp = t->children[10];
		
			int count = 0;
		
			if(temp->num_children==3)
			{
				int x = processIntegerList(temp->children[0]);
				if(x==0)
				{
						printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
					printf(" lexeme and type of second operator: ***, ");
					printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
					return;
				}
				ptr2->dimension=x;
				temp = temp->children[2];
				count++;
			}
	
			while(temp->num_children==3)
			{
				ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
				ptr2 = ptr2->next;
				int x = processIntegerList(temp->children[0]);
				if(x==0)
				{
						printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
				}
				ptr2->dimension = x;
				ptr2->next=NULL;
				temp = temp->children[2];
				count++;
			}
		
			ptr2->next = (jagged3d_R3_range *) malloc(sizeof(jagged3d_R3_range));
			ptr2 = ptr2->next;
			int x = processIntegerList(t->children[11]);
			if(x==0)
			{
					printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Empty value list");
				return;
			}
			count++;
			ptr2->dimension = x;
			ptr2->next=NULL;
			
			ptr->R3_size = atoi(t->children[6]->terminal);
			if(ptr->R3_size != count)
			{
				printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  t->children[0]->line_no, "***");
				printf(" lexeme and type of second operator: ***, ");
				printf("depth: %d, Error: %s\n\n", depth, "Initialization list size mismatch");
				return;
			}
			ptr->next = NULL;
			t=t->children[13];
		}
		 
		
	    t->typeExprT.data_type = item.data_type;
        t->typeExprT.array_type = item.data_type;
        t->typeExprT.type_expression = item.type_expression;
		t->typeExprT.is2Djagged = item.is2Djagged;

    for(int i=0;i<*count;i++){
        // printf("\n%s",varList[i]);
        typeExpressionTable item_i = malloc(sizeof(struct typeExprTable));
        strcpy(item_i->name,varList[i]);
        
        if(checkDeclared(start,idNode,item_i->name, depth)){
            free(item_i);
            continue ;
        }
        
        item_i->data_type = item.data_type;
        item_i->array_type = item.array_type;
        item_i->type_expression = item.type_expression;
        item_i->is2Djagged = item.is2Djagged;
        // T->next = item_i;
        // T = T->next;
        head->next = item_i;
        head = head->next;
        if(start==NULL){
            start = malloc(sizeof(struct typeExprTable)); 
            start->next = head;
        }
        // printTypeExpressionTable(item_i);
    }

        // // T->next = item;
        // // T = T->next;
        // head->next = item;
        // head = head->next;

        // if(start==NULL){
        //     start = malloc(sizeof(struct typeExprTable)); 
        //     start->next = head;
        // }

}

void processMultJagged(ParseTree *t, typeExpressionTable T,int depth){
    //printf("Inside multiple jagged\n");
    if(t){
		if(!strcmp(t->children[10]->symbol, "Declare_2D_jagged"))
			processMult2DJagged(t, T, depth);
		else 
			processMult3DJagged(t, T, depth);
	}
}

void processSingleJagged(ParseTree *t, typeExpressionTable T, int depth){
    if(t){
		if(!strcmp(t->children[5]->symbol, "Declare_2D_jagged"))
			processSingle2DJagged(t, T, depth);
		else
			processSingle3DJagged(t, T, depth);
	}
}




void printTypeExpressionTable (typeExpressionTable T){
    // T is head here not next of head
    // T = T->next;
    printf("\n");
    while(T){
        printf("Field 1 (Variable Name) : %s | ",T->name);
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

             printf(" basicElementType = Integer>");
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
                         printf("%d,",r2->R3_size);
                    jagged3d_R3_range* r3 = r2->R3_dimension_list;
                    printf(" [ ");
                    while(r3){
                        printf("%d ",r3->dimension);
                        r3=r3->next;
                    }
                    printf("] ");
                    r2=r2->next;
                }
                printf(")");
            }

              printf(" basicElementType = Integer>");
        
        }
        printf("\n\n");

        T = T->next;
    }
}

void printTypeExpression(typeExpressionTable T)
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


char * comparetypeExpressionTables (typeExprTable A, typeExprTable B)
{	
     /*printTypeExpression(&A);
	 printf("\n");
	 printTypeExpression(&B);
	 printf("\n");*/
	if(A.data_type!=B.data_type)
	{
		return "Diff data types";
	}
	
	if(A.data_type==0)
	{
		if((A.type_expression).prim.prim_type != (B.type_expression).prim.prim_type)
		return "Diff primitive types";
	}
	
	if(A.data_type==1)
	{
		if(A.type_expression.rect.dimension!=B.type_expression.rect.dimension)
		{
			return "Rect array diff dimension";
		}
		
		if(A.array_type==0 && B.array_type==0)
		{
			rectRange * range_A = A.type_expression.rect.range;
			rectRange * range_B = B.type_expression.rect.range;
			
			while(range_A!=NULL)
			{
				if(strcmp(range_A->low, range_B->low) || strcmp(range_A->high, range_B->high))
					return "Rect array dimension mismatch";
				range_A = range_A->next;
				range_B = range_B->next;
			}
			
			
		}
		
		return "Correct";
	}
	
	if(A.data_type==2)
    {
		if(A.is2Djagged^B.is2Djagged)
			return "Jagged array dim mismatch";
		
		
			if(A.is2Djagged)
			{
				twoDimJagged twoDimJagged_A = A.type_expression.jagged_twod;
				twoDimJagged twoDimJagged_B = B.type_expression.jagged_twod;
				if(twoDimJagged_A.R1_lo!=twoDimJagged_B.R1_lo || twoDimJagged_A.R1_hi!=twoDimJagged_B.R1_hi)
					return "Jagged 2D R1 mismatch";
				
				jagged2drange * ptr_A = twoDimJagged_A.dimension_list;
				jagged2drange * ptr_B = twoDimJagged_B.dimension_list;
				
				while(ptr_A)
				{
					if(ptr_A->dimension!=ptr_B->dimension)
						return "Jagged 2D R2 mismatch";
					ptr_A=ptr_A->next;
					ptr_B=ptr_B->next;
				}
				
				return "Correct";
			}
			
			else
			{
				threeDimJagged threeDimJagged_A = A.type_expression.jagged_threed;
				threeDimJagged threeDimJagged_B = B.type_expression.jagged_threed;
				
				if(threeDimJagged_A.R1_lo != threeDimJagged_B.R1_lo || threeDimJagged_A.R1_hi != threeDimJagged_B.R1_hi)
					return "Jagged 3D R1 mismatch";
				
				jagged3d_R2_range * R2_range_A =  threeDimJagged_A.R2_dimension_list;
				jagged3d_R2_range * R2_range_B =  threeDimJagged_B.R2_dimension_list;
				
				while(R2_range_A)
				{
					if(R2_range_A->R3_size!=R2_range_B->R3_size)
					{
						return "Jagged 3D R2 mismatch";
					}
					
					jagged3d_R3_range * R3_range_A = R2_range_A->R3_dimension_list;
					jagged3d_R3_range * R3_range_B = R2_range_B->R3_dimension_list;
					
					while(R3_range_A)
					{
						if(R3_range_A->dimension!=R3_range_B->dimension)
							return "Jagged 3D R3 mismatch";
						
						R3_range_A = R3_range_A->next;
					    R3_range_B = R3_range_B->next;
					}
					
					R2_range_A = R2_range_A->next;
					R2_range_B = R2_range_B->next;
				}
				
				return "Correct";
			}
	}
	
	return "Correct";
}

void printAssignmentError(int line_no, char * operator, char* lexeme1, typeExprTable type1,  char* lexeme2, typeExprTable type2, int depth, char * message)
{
	printf("Line number: %d, operator: %s lexeme and type of first operand: ", line_no, operator);
	printTypeExpression (&type1);
	printf(" lexeme and type of second operand: ");
	printTypeExpression (&type2);
	printf("depth: %d, Error: %s\n\n", depth, message);
}

int getNumItemList (ParseTree * t)
{
	if(!strcmp(t->children[0]->symbol, "epsilon"))
		return 0;
	else
	{
		return getNumItemList(t->children[1])+1;
	}
}


typeExprTable getTypeExpression(char * identifer, typeExpressionTable T, int line_no, int depth)
{
	typeExpressionTable ptr = T->next;
	
	while(ptr)
	{
		if(!strcmp(ptr->name, identifer))
		{
			return *ptr;
		}
		
		ptr = ptr->next;
	}
	/*
	printf("Line number: %d, operator: %s, lexeme and type of first operator: ***,",  line_no, "***");
	printf(" lexeme and type of second operator: ***, ");
	printf("depth: %d, Error: Variable not found\n\n", depth);	*/
	
	return *T;
}

int generate_assign_type(ParseTree *t, typeExpressionTable T, int depth)
{
	T = start->next;
	int x;
	//printf("yo %s\n", t->symbol);
	if(!strcmp(t->symbol, "Assignment_statement"))
	{
		x = generate_assign_type(t->children[0], T, depth+1);
		x |= generate_assign_type(t->children[2], T, depth+1);
		
		if(x)
			return 1;
		
		t->typeExprT = (t->children[2])->typeExprT;
		
		strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
		strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[1]->terminal);
		strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[2]->typeExprT.name);
		
		t->line_no = t->children[0]->line_no;
			
		char * message = comparetypeExpressionTables(t->children[0]->typeExprT, t->typeExprT);
		
		if(strcmp(message, "Correct"))
		{
			if(!strcmp(t->children[0]->symbol, "Array_variable"))
			{
				printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->symbol, t->children[0]->typeExprT,  t->children[2]->symbol, t->children[2]->typeExprT, depth, message);
			}
			
			else
				printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->terminal, t->children[0]->typeExprT,  t->children[2]->symbol, t->children[2]->typeExprT, depth, message);
			
			return 1;
		}
	}
	
	else if(!strcmp(t->symbol, "Expression"))
	{
		x = generate_assign_type(t->children[0], T, depth+1);
		
		if(x)
			return 1;
		
		t->typeExprT = (t->children[0])->typeExprT;
		strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
	
		t->line_no = 0;
		for(int j=0; j<(t)->num_children; ++j)
		{
			if( (t)->children[j]->line_no > (t)->line_no)
				(t)->line_no = (t)->children[j]->line_no;
		}
	}
	
	else if(!strcmp(t->symbol, "Boolean_expression"))
	{
		if(t->num_children==1)
		{
			x = generate_assign_type(t->children[0], T, depth+1);
			
			if(x)
				return 1;
			
			t->typeExprT = (t->children[0])->typeExprT;
			t->line_no = 0;
			for(int j=0; j<(t)->num_children; ++j)
			{
				if( (t)->children[j]->line_no > (t)->line_no)
					(t)->line_no = (t)->children[j]->line_no;
			}
			strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
		
		}
		
		else
		{
			x = generate_assign_type(t->children[0], T, depth+1);
			x |= generate_assign_type(t->children[2], T, depth+1);
			
			if(x)
				return 1;
			
			t->typeExprT = (t->children[2])->typeExprT;
			t->line_no = 0;
				strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
				strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[1]->terminal);
		strcat(t->typeExprT.name, " ");
		     strcat(t->typeExprT.name, t->children[2]->typeExprT.name);
			for(int j=0; j<(t)->num_children; ++j)
			{
				if( (t)->children[j]->line_no > (t)->line_no)
					(t)->line_no = (t)->children[j]->line_no;
			}
			
			char * message = comparetypeExpressionTables(t->children[0]->typeExprT, t->typeExprT);
			
			if(strcmp(message, "Correct"))
			{
				printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->symbol, t->children[0]->typeExprT,  t->children[2]->symbol, t->children[2]->typeExprT, depth, message);
				return 1;
			}
		}
	}
	
	else if(!strcmp(t->symbol, "And_expression"))
	{
		if(t->num_children==1)
		{
			t->line_no = t->children[0]->line_no;
			t->children[0]->typeExprT = getTypeExpression((t->children[0])->terminal, T, t->children[0]->line_no, depth);
			t->typeExprT = (t->children[0])->typeExprT;
			strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
		
		}
		
		else
		{
			x = generate_assign_type(t->children[2], T, depth+1);
			
			if(x)
				return 1;
			
		    t->typeExprT = (t->children[2])->typeExprT;
		    t->line_no = t->children[0]->line_no;
		
		    typeExprTable LHS = getTypeExpression(t->children[0]->terminal, T, t->children[0]->line_no, depth);
			
			 strcpy(t->typeExprT.name, LHS.name);
			 strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[1]->terminal);
		strcat(t->typeExprT.name, " ");
		     strcat(t->typeExprT.name, t->children[2]->typeExprT.name);
		
			char * message = comparetypeExpressionTables(LHS, t->typeExprT);	
		
			if(strcmp(message, "Correct"))
			{
				printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->terminal, LHS,  t->children[2]->symbol, t->children[2]->typeExprT, depth, message);
				return 1;
			}
			
		
			if(t->typeExprT.data_type!=0 || t->typeExprT.type_expression.prim.prim_type != 2)
			{
				printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->terminal, LHS,  t->children[2]->symbol, t->children[2]->typeExprT, depth, "Boolean opeartion on non-bool type");
				return 1;
			}
		}
	}
	
	else if(!strcmp(t->symbol, "Arithmetic_expression"))
	{
		if(t->num_children==1)
		{
			x = generate_assign_type(t->children[0], T, depth+1);
			
			if(x)
				return 1;
			
			t->typeExprT = (t->children[0])->typeExprT;
			t->line_no = 0;
				strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
		
			for(int j=0; j<(t)->num_children; ++j)
			{
				if( (t)->children[j]->line_no > (t)->line_no)
					(t)->line_no = (t)->children[j]->line_no;
			}
		}
		
		else
		{
			x = generate_assign_type(t->children[0], T, depth+1);
			x |= generate_assign_type(t->children[2], T, depth+1);
			
			if(x)
				return 1;
			
			t->typeExprT = (t->children[2])->typeExprT;
			t->line_no = 0;
				strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
				strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[1]->terminal);
		strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[2]->typeExprT.name);
			for(int j=0; j<(t)->num_children; ++j)
			{
				if( (t)->children[j]->line_no > (t)->line_no)
					(t)->line_no = (t)->children[j]->line_no;
			}
			
			char * message = comparetypeExpressionTables(t->children[0]->typeExprT, t->typeExprT);
			
			if(strcmp(message, "Correct"))
			{
				printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->symbol, t->children[0]->typeExprT,  t->children[2]->symbol, t->children[2]->typeExprT, depth, message);
				return 1;
			}
		}
	}
	
	else if(!strcmp(t->symbol, "Mul_div_expression"))
	{
		if(t->num_children==1)
		{
			x = generate_assign_type(t->children[0], T, depth+1);
			
			
			if(x)
				return 1;
			
			t->typeExprT = (t->children[0])->typeExprT;
			t->line_no = 0;
				strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
				
					//	printTypeExpression(&(t->children[0]->typeExprT));
			//printTypeExpression(&(t->typeExprT));
			//printf("peace\n");
	
			for(int j=0; j<(t)->num_children; ++j)
			{
				if( (t)->children[j]->line_no > (t)->line_no)
					(t)->line_no = (t)->children[j]->line_no;
			}
		}
		
		else
		{
			x = generate_assign_type(t->children[0], T, depth+1);
			x |=generate_assign_type(t->children[2], T, depth+1);
			
			if(x)
				return 1;
			
			t->typeExprT = (t->children[2])->typeExprT;
			t->line_no = 0;
				strcpy(t->typeExprT.name, (t->children[0])->typeExprT.name);
				strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[1]->terminal);
		strcat(t->typeExprT.name, " ");
		strcat(t->typeExprT.name, t->children[2]->typeExprT.name);
			for(int j=0; j<(t)->num_children; ++j)
			{
				if( (t)->children[j]->line_no > (t)->line_no)
					(t)->line_no = (t)->children[j]->line_no;
			}
			
			//printTypeExpression(&(t->children[0]->typeExprT));
			//printTypeExpression(&(t->typeExprT));
			
			char * message = comparetypeExpressionTables(t->children[0]->typeExprT, t->typeExprT);
			
			if(strcmp(message, "Correct"))
			{
				if(!strcmp(t->children[0]->symbol, "Array_variable"))
				{
					printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->symbol, t->children[0]->typeExprT,  t->children[2]->symbol, t->children[2]->typeExprT, depth, message);
				}
				
				else
					printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->terminal, t->children[0]->typeExprT,  t->children[2]->symbol, t->children[2]->typeExprT, depth, message);
				
				return 1;
			}
			
			
			if(!strcmp(t->children[1]->symbol, "div_op"))
			{
				if(t->children[0]->typeExprT.data_type!=0)
				{
					printAssignmentError(t->line_no, t->children[1]->terminal, t->children[0]->terminal, t->children[0]->typeExprT,  t->children[2]->symbol, t->children[2]->typeExprT, depth, "Div op to non-primitive type");
					return 1;
				}
				t->typeExprT.data_type = 0;
				t->typeExprT.array_type = 2;
				t->typeExprT.type_expression.prim.prim_type = 1;
			}
		}
	}
	
	else if(!strcmp(t->symbol, "Item"))
	{
		if(!strcmp(t->children[0]->symbol, "Array_variable"))
		{
			x = generate_assign_type(t->children[0], T, depth+1);
			
			t->typeExprT = t->children[0]->typeExprT;
		}
		
		else
		{
			if(!strcmp(t->children[0]->symbol, "id"))
			{
				t->typeExprT = getTypeExpression(t->children[0]->terminal, T, t->children[0]->line_no, depth);
				//printTypeExpression(&(t->typeExprT));
			}
			
			else
			{
				t->typeExprT.data_type = 0;
				t->typeExprT.array_type = 2;
				t->typeExprT.type_expression.prim.prim_type = 0;
				strcpy(t->typeExprT.name, t->children[0]->terminal);
			}
		}
		
		t->line_no = t->children[0]->line_no;
	}
	
	else if(!strcmp(t->symbol, "Array_variable"))
	{
		int correct_number_dimensions;
		
		t->line_no = t->children[0]->line_no;
		t->typeExprT = getTypeExpression(t->children[0]->terminal, T, t->children[0]->line_no, depth);
		
		if(t->typeExprT.data_type==1)
		{
			correct_number_dimensions = t->typeExprT.type_expression.rect.dimension;
		}
		
		else if(t->typeExprT.is2Djagged)
		{
			correct_number_dimensions = 2;
		}
		
		else
			correct_number_dimensions = 3;
		
		if(correct_number_dimensions!=getNumItemList(t->children[3])+1)
		{
			printf("Line number: %d, operator: %s, lexeme of first operand: %s", t->line_no, "[ ]", "Array_variable, ");
			printf("type of first operand: ");
			printTypeExpression(&(t->typeExprT));
			printf(" lexeme and type of second operand: ***, ");
			printf("depth: %d, Error: %s\n\n", depth, "Array access incorrect dimensions");
		}
		
		t->typeExprT.data_type = 0;
		t->typeExprT.array_type = 2;
		t->typeExprT.type_expression.prim.prim_type = 0;
		
	}
	else
		printf("Unknown symbol type!\n");
	return 0;
}
