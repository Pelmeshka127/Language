#include "front_end.h"
#include <ctype.h>

//-------------------------------------------------------------------------------//

tree_node * Get_General(Text_Info * onegin)
{
    tree_node *node = Get_Assignment(onegin);

    assert(**onegin->pointers == '\0');

    // printf("%d\n", onegin->symbols_count);
    // printf("%d\n", onegin->char_num);

    Tree_Dump_Node(node);

    return node;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Add_Or_Sub(Text_Info * onegin)
{
    tree_node *node_1 = Get_Mul_Or_Div(onegin);
    Skip_Spaces(onegin);

    while (**onegin->pointers == '+' || **onegin->pointers == '-')
    {
        Tree_Dump_Node(node_1);

        char op = **onegin->pointers;
        (*onegin->pointers)++;
        onegin->char_num++;

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
        (*onegin->pointers)++;
        onegin->char_num++;

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
    tree_node *node_1 = Get_Func(onegin);
    Skip_Spaces(onegin);

    while (**onegin->pointers == '^')
    {
        Tree_Dump_Node(node_1);

        (*onegin->pointers)++;
        onegin->char_num++;

        Skip_Spaces(onegin);

        tree_node *node_2 = Get_Func(onegin);

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
        (*onegin->pointers)++;
        onegin->char_num++;

        node_1 = Get_Add_Or_Sub(onegin);
        assert(**onegin->pointers == ')');

        (*onegin->pointers)++;
        onegin->char_num++;
    }

    else
        node_1 = Get_Var(onegin);
    
    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Var(Text_Info * onegin)
{
    if (**onegin->pointers == '\0')
    {
        // printf("%s\n", *onegin->pointers);
        if (onegin->char_num != onegin->symbols_count)
        {
            onegin->pointers++;        
            onegin->char_num++;
            // printf("%s\n", *onegin->pointers);
        }
        else
            return nullptr;
    }

    char *var = (char *)calloc (Max_Size, sizeof(char));
    
    sscanf(*onegin->pointers, "%s", var);
    // printf("%s\n", var);
    // printf("%d\n", strlen(var));
    if (var[strlen(var) - 1] == ')')
        var[strlen(var) - 1] = '\0';

    int type = Is_Var(var);

    if (type == Var_Type)
    {
        (*onegin->pointers)+=strlen(var);
        onegin->char_num += strlen(var);
        while (**onegin->pointers == ' ')
        {
            (*onegin->pointers)++;
            onegin->char_num++;
        }
        return New_Var_New(var);
    }

    return Get_Number(onegin);
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
    while (isspace(**onegin->pointers))
    {
        (*onegin->pointers)++;
        onegin->char_num++;
    }
}

tree_node * Get_Assignment(Text_Info * onegin)
{
    tree_node *node_1 = Get_Var(onegin);
    Skip_Spaces(onegin);

    while (**onegin->pointers == '=')
    {
        assert(node_1->type == Var_Type);
        Tree_Dump_Node(node_1);

        (*onegin->pointers)++;
        onegin->char_num++;
        
        Skip_Spaces(onegin);

        tree_node *node_2 = Get_Add_Or_Sub(onegin);

        Tree_Dump_Node(node_2);

        //node_1->data = Eval(node_2);

        node_1 = EQUAL(node_1, node_2);

        node_1 = New_Connect_Type(node_1, Get_Assignment(onegin));

        Tree_Dump_Node(node_1);
    }

    return node_1;
}