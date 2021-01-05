/*
 *  *  *  *Group no: 8
 *   *   *   *Jai Doshi 2018A7PS0012P
 *    *    *    *Aditya Deshmukh 2018A7PS0246P
 *     *     *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *      *      *
 *       *       *       * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stacknode{
    char data [50];
    struct stacknode* next;
}stacknode;


typedef struct Stack{
    stacknode* top; 
    int size;
}Stack;


void push(char data[], Stack* st);

void pop(Stack* st);

stacknode* top(Stack* st);

Stack* createStackCopy(Stack* st);

void deleteStack(Stack* st);
