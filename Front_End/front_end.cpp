#include "front_end.h"
#include <ctype.h>

//-------------------------------------------------------------------------------//

tree_node * Get_General(Text_Info * onegin)
{
    tree_node *node = Get_Type(onegin);

    assert(**onegin->pointers == '\0');

    // printf("%d\n", onegin->symbols_count);
    // printf("%d\n", onegin->char_num);

    return node;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Add_Or_Sub(Text_Info * onegin)
{
    Skip_Spaces(onegin);
    tree_node *node_1 = Get_Mul_Or_Div(onegin);
    Skip_Spaces(onegin);

    while (**onegin->pointers == '+' || **onegin->pointers == '-')
    {
        Tree_Dump_Node(node_1);

        char op = **onegin->pointers;
        Move_Pointers(onegin);

        Skip_Spaces(onegin);

        tree_node *node_2 = Get_Mul_Or_Div(onegin);

        Tree_Dump_Node(node_2);

        if (op == '+')
            node_1 = ADD(node_1, node_2);

        else
            node_1 = SUB(node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Mul_Or_Div(Text_Info * onegin)
{
    tree_node *node_1 = Get_Power(onegin);
    Skip_Spaces(onegin);

    while (**onegin->pointers == '*' || **onegin->pointers == '/')
    {
        Tree_Dump_Node(node_1);

        char op = **onegin->pointers;
        Move_Pointers(onegin);

        Skip_Spaces(onegin);

        tree_node *node_2 = Get_Power(onegin);

        Tree_Dump_Node(node_2);

        if (op == '*')
            node_1 = MUL(node_1, node_2);

        else
            node_1 = DIV(node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Power(Text_Info * onegin)
{
    tree_node *node_1 = Get_Log_Operator(onegin);
    Skip_Spaces(onegin);

    while (**onegin->pointers == '^')
    {
        Tree_Dump_Node(node_1);

        Move_Pointers(onegin);

        Skip_Spaces(onegin);

        tree_node *node_2 = Get_Log_Operator(onegin);

        Tree_Dump_Node(node_2);

        node_1 = POW(node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Func(Text_Info * onegin)
{
    int op = Is_Func(onegin);

    if (op)
    {
        tree_node *node = Get_Petrovich(onegin);

        switch(op)
        {
            case Op_Sin:
                return SIN(node);
            case Op_Cos:
                return COS(node);
            case Op_Ln:
                return LN(node);
            case Op_Exp:
                return EXP(node);
            default:
                break;
        }
    }

    return Get_Petrovich(onegin);
}

//-------------------------------------------------------------------------------//

int Is_Func(Text_Info * onegin)
{
    if (**onegin->pointers == 's' && *(*onegin->pointers + 1) == 'i' && *(*onegin->pointers + 2) == 'n' && *(*onegin->pointers + 3) == '(')
    {
        *onegin->pointers += 3;
        onegin->char_num += 3;
        return Op_Sin;
    }

    else if (**onegin->pointers == 'c' && *(*onegin->pointers + 1) == 'o' && *(*onegin->pointers + 2) == 's' && *(*onegin->pointers + 3) == '(')
    {
        *onegin->pointers += 3;
        onegin->char_num +=3 ;
        return Op_Cos;
    }

    else if (**onegin->pointers == 'l' && *(*onegin->pointers + 1) == 'n' && *(*onegin->pointers + 2) == '(')
    {
        *onegin->pointers += 2;
        onegin->char_num += 2;
        return Op_Ln;
    }

    else if (**onegin->pointers == 'e' && *(*onegin->pointers + 1) == 'x' && *(*onegin->pointers + 2) == 'p' && *(*onegin->pointers + 3) == '(')
    {
        *onegin->pointers += 3;
        onegin->char_num += 3;
        return Op_Exp;
    }

    else 
        return No_Error;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Petrovich(Text_Info * onegin)
{
    tree_node *node_1 = nullptr;

    if (**onegin->pointers == '(')
    {
        Move_Pointers(onegin);

        node_1 = Get_Add_Or_Sub(onegin);
        assert(**onegin->pointers == ')');

        Move_Pointers(onegin);
    }

    else
        node_1 = Get_Var(onegin);
    
    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Var(Text_Info * onegin)
{
    Skip_Spaces(onegin);

    char *var = (char *)calloc (Max_Size, sizeof(char));
    
    sscanf(*onegin->pointers, "%s", var);
    if (var[strlen(var) - 1] == ')')
        var[strlen(var) - 1] = '\0';

    int type = Is_Var(var);

    if (type == Var_Type)
    {
        (*onegin->pointers)+=strlen(var);
        onegin->char_num += strlen(var);
        while (**onegin->pointers == ' ')
        {
            Move_Pointers(onegin);
        }
        return New_Var_New(var);
    }

    if (Is_Digit(var))
        return Get_Number(onegin);

    return nullptr;
}

//-------------------------------------------------------------------------------//

int Is_Var(char *var)
{
    int is_var = Var_Type;

    for (int ch = 0; ch < strlen(var); ch++)
    {
        if ( !(isalpha(var[ch])) )
        {
            is_var = 0;
            break;
        }
    }

    return is_var;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Number(Text_Info * onegin)
{
    int value = 0;
    int len = 0;

    sscanf(*onegin->pointers, "%d%n", &value, &len);
    
    assert(len);

    *onegin->pointers += len;
    onegin->char_num += len;

    return New_Num(value);
}

//-------------------------------------------------------------------------------//

void Skip_Spaces(Text_Info * onegin)
{
    while (isspace(**onegin->pointers) || **onegin->pointers == '\0')
    {
        Move_Pointers(onegin);
    }
}

//-------------------------------------------------------------------------------//

tree_node * Get_Assignment(Text_Info * onegin)
{
    tree_node *node_1 = Get_Var(onegin);
    Skip_Spaces(onegin);

    while (**onegin->pointers == '=')
    {
        assert(node_1->type == Var_Type);
        Tree_Dump_Node(node_1);

        Move_Pointers(onegin);
        
        Skip_Spaces(onegin);

        tree_node *node_2 = Get_Add_Or_Sub(onegin);

        Tree_Dump_Node(node_2);

        //node_1->data = Eval(node_2);

        node_1 = EQUAL(node_1, node_2);

        node_1 = New_Connect_Type(node_1, Get_Type(onegin));

        Tree_Dump_Node(node_1);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_If(Text_Info * onegin)
{
    while (**onegin->pointers != '(')
        Move_Pointers(onegin);
    
    Move_Pointers(onegin);

    tree_node *node_1 = Get_Add_Or_Sub(onegin);
    tree_node *node_2 = nullptr;

    while (!(isalpha(**onegin->pointers)))
        Move_Pointers(onegin);

    char word[Max_Length] = "";

    sscanf(*onegin->pointers, "%s", word);

    if (strcmp(word, "start_sex") == 0)
    {
        (*onegin->pointers)+=strlen(word);
        onegin->char_num+=strlen(word);
        node_2 = Get_If_Body(onegin);
    }

    sscanf(*onegin->pointers, "%s", word);

    if (strcmp(word, "end_sex") == 0)
    {
        (*onegin->pointers)+=strlen(word);
        onegin->char_num+=strlen(word);
        return IF(node_1, node_2);
    }

    else
        return nullptr;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Type(Text_Info * onegin)
{
    if (**onegin->pointers == '\0')
    {
        if (onegin->char_num != onegin->symbols_count)
        {
            Move_Pointers(onegin);
        }
        else
            return nullptr;
    }

    char *tokens = *onegin->pointers;

    char word[Max_Size] = "";
    
    sscanf(tokens, "%s", word);
    // printf("%s\n", tokens);

    if (strcmp(word, "if") == 0 && strchr(tokens, '(') != nullptr && strchr(tokens, ')') != nullptr)
    {
        return Get_If(onegin);
    }

    else
        return Get_Assignment(onegin);

}

//-------------------------------------------------------------------------------//

void Move_Pointers(Text_Info * onegin)
{
    (*onegin->pointers)++;
    onegin->char_num++;
}

//-------------------------------------------------------------------------------//

tree_node * Get_If_Body(Text_Info * onegin)
{   
    Move_Pointers(onegin);

    tree_node *body = Get_Type(onegin);

    return body;
}

//-------------------------------------------------------------------------------//

int Is_Digit(char *word)
{
    int is_digit = 1;
    for (int i = 0; i < strlen(word); i++)
    {
        if (!(isdigit(word[i])))
            is_digit = 0;
    }

    return is_digit;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Log_Operator(Text_Info *onegin)
{
    Skip_Spaces(onegin);
    tree_node *node_1 = Get_Func(onegin);
    Skip_Spaces(onegin);

    char operation[Max_Size] = "";
    sscanf(*onegin->pointers, "%s", operation);

    printf("%s\n", operation);

    int log_op = Is_Log_Operation(operation);
    if (log_op)
    {
        Tree_Dump_Node(node_1);

        (*onegin->pointers)+=strlen(operation);
        onegin->char_num+=strlen(operation);

        Skip_Spaces(onegin);

        tree_node *node_2 = Get_Func(onegin);

        Tree_Dump_Node(node_2);

        node_1 = Tree_New_Node(Op_Type, log_op, operation, node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

int Is_Log_Operation(char *operation)
{
    if (strcmp("<", operation) == 0)
        return Op_Less;
    else if (strcmp("<=", operation) == 0)
        return Op_Less_Eq;
    else if (strcmp(">", operation) == 0)
        return Op_More;
    else if (strcmp(">=", operation) == 0)
        return Op_More_Eq;
    else if (strcmp("==", operation) == 0)
        return Op_ChecK;
    else if (strcmp("ass", operation) == 0)
        return Op_And;
    else if (strcmp("oral_sex", operation) == 0)
        return Op_Or;

    else
        return 0;
}

//-------------------------------------------------------------------------------//