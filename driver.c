/*
 *  *Group no: 8
 *   *Jai Doshi 2018A7PS0012P
 *    *Aditya Deshmukh 2018A7PS0246P
 *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *
 *       * */
#include <stdio.h>
#include "typeExpressionTable.h"

int main(int argc,char **argv){

	    if(argc==1){
                printf("\nERROR invaild file entered in Command Line !!\n\nExiting ... \n\n");
                exit(1);
            }
	start = NULL;
	typeExpressionTable temp=malloc(sizeof(struct typeExprTable)); 

	head = malloc(sizeof(struct typeExprTable));
	
	
    ParseTree *pt=NULL;
    int istraverseparsetree=0;
    int option;
    do
    {
        printf("\nPlease enter an option (0-4) : ");
        scanf("%d",&option);
        if(option==0)
        {
            printf("\nExiting ...\n");
        }
        else if(option==1)
        {
            printf("\nGenerating Parse Tree ...\n");
            Grammar G;
            G=readGrammar(G);
            TokenStream *ts=(TokenStream *)malloc(sizeof(TokenStream));
            pt=(ParseTree *)malloc(sizeof(ParseTree));
            
    
            
            fp=fopen(argv[1],"r");

            ts=tokeniseSourcecode(ts);
            pt=createParseTree(pt,ts,G);
        }
        else if(option==2)
        {
            printf("\nGenerating Type Expression Table ...\n");
            if(pt!=NULL)
            {traverseParseTree(pt,temp);
                istraverseparsetree=1;
            }
            else
                printf("Parse Tree is Not Created");
        }
        else if(option==3)
        {
            printf("\nPrinting the Parse Tree ...\n\n");
                 if(pt!=NULL)
                printParseTree(pt);
                 else
                printf("Parse Tree is Not Created");
        }
        else if(option==4)
        {
            printf("\nPrinting the Type Expression Table ...\n\n");
            if(istraverseparsetree==1)
            printTypeExpressionTable(start->next);
            else
              printf("Parse Tree is Not Traversed");  
        }
        else{
            printf("\nERROR !! Invalid option entered ! Please enter option between 0 - 4 (inclusive both values)\n");
        }
    }while(option!=0);
    return 0;
}
