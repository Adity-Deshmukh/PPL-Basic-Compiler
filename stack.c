/*
 *  *  *Group no: 8
 *   *   *Jai Doshi 2018A7PS0012P
 *    *    *Aditya Deshmukh 2018A7PS0246P
 *     *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *      *
 *       *       * */
#include "stack.h"

void push(char data[], Stack* st)
{

    if(st->top==NULL)
        st->size=0;    

    stacknode* new_node = (stacknode*)malloc(sizeof(stacknode));
    strcpy(new_node->data,data);
    new_node->next=NULL;


    stacknode* prev_top = st->top;
    if(prev_top)
        new_node->next=prev_top;
    st->top=new_node;

    st->size++;
}

void pop(Stack* st)
{
    if(st->size==0)
    {
        printf("ERROR! Stack is empty ! Unable to pop elements from Stack");
        exit(1);
    }

    stacknode* top = st->top;
    st->top = top->next;
    free(top);

    st->size--;
}

stacknode* top(Stack* st){
    if(st->size==0)
    {
        printf("ERROR! Stack is empty ! Unable to seek top from the Stack");
        exit(1);
    }

    return st->top;
}


Stack* createStackCopy(Stack* st)
{
    Stack* copy = (Stack*)malloc(sizeof(Stack));
    copy->size=st->size;

    stacknode* node = (stacknode*)malloc(sizeof(stacknode));
    stacknode* head = node;

    stacknode* traverse = st->top;
    while(traverse)
    {
        strcpy(node->data,traverse->data);
        node->next=NULL;
        traverse = traverse->next;
        if(traverse)
        {
            node->next = (stacknode*)malloc(sizeof(stacknode));
            node=node->next;
        }
    }

    copy->top = head;
    return copy;
}

void deleteStack(Stack* st)
{
    stacknode* traverse = st->top;
    while(traverse)
    {
        stacknode* next = traverse->next;
        free(traverse);
        traverse = next;
    }
    free(st);
}

