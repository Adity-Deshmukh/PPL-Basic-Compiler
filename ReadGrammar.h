/*
 *  *Group no: 8
 *   *Jai Doshi 2018A7PS0012P
 *    *Aditya Deshmukh 2018A7PS0246P
 *     *Shah Jainam Satishkumar 2018A7PS0212P
 *      *
 *       * */
typedef struct ProductionRuleNode{
char symbol[50];
struct ProductionRuleNode *next;
} ProductionRuleNode;

typedef struct ProductRuleLinkedlist{
char nonterminal[50];
ProductionRuleNode *head;
} ProductRuleLinkedlist;

typedef struct Grammar{
int size;
ProductRuleLinkedlist *arr;
} Grammar;

Grammar readGrammar(Grammar G);

