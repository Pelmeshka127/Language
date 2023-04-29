#include "front_end.h"

//-------------------------------------------------------------------------------//

tree_node * Get_General(char **str)
{
    tree_node *node = Get_Add_Or_Sub(str);

    assert(**str == '\0');

    Tree_Dump_Node(node);

    return node;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Add_Or_Sub(char **str)
{
    tree_node *node_1 = Get_Mul_Or_Div(str);

    while (**str == '+' || **str == '-')
    {
        Tree_Dump_Node(node_1);

        char op = **str;
        (*str)++;

        tree_node *node_2 = Get_Mul_Or_Div(str);

        Tree_Dump_Node(node_2);

        if (op == '+')
            node_1 = ADD(node_1, node_2);

        else
            node_1 = SUB(node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Mul_Or_Div(char **str)
{
    tree_node *node_1 = Get_Power(str);

    while (**str == '*' || **str == '/')
    {
        Tree_Dump_Node(node_1);

        char op = **str;
        (*str)++;

        tree_node *node_2 = Get_Power(str);

        Tree_Dump_Node(node_2);

        if (op == '*')
            node_1 = MUL(node_1, node_2);

        else
            node_1 = DIV(node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Power(char **str)
{
    tree_node *node_1 = Get_Func(str);

    while (**str == '^')
    {
        Tree_Dump_Node(node_1);

        (*str)++;

        tree_node *node_2 = Get_Func(str);

        Tree_Dump_Node(node_2);

        node_1 = POW(node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Func(char **str)
{
    int op = Is_Func(str);

    if (op)
    {
        tree_node *node = Get_Petrovich(str);

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

    return Get_Petrovich(str);
}

//-------------------------------------------------------------------------------//

int Is_Func(char **str)
{
    if (**str == 's' && *(*str + 1) == 'i' && *(*str + 2) == 'n')
    {
        *str += 3;
        return Op_Sin;
    }

    else if (**str == 'c' && *(*str + 1) == 'o' && *(*str + 2) == 's')
    {
        *str += 3;
        return Op_Cos;
    }

    else if (**str == 'l' && *(*str + 1) == 'n')
    {
        *str += 2;
        return Op_Ln;
    }

    else if (**str == 'e' && *(*str + 1) == 'x' && *(*str + 2) == 'p')
    {
        *str += 3;
        return Op_Exp;
    }

    else 
        return No_Error;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Petrovich(char **str)
{
    tree_node *node_1 = nullptr;

    if (**str == '(')
    {
        (*str)++;
        node_1 = Get_Add_Or_Sub(str);
        assert(**str == ')');
        (*str)++;
    }

    else
        node_1 = Get_Var(str);
    
    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Var(char **str)
{
    int type = Is_Var(str);

    if (type == Var_X)
        return New_Var(Var_X);

    return Get_Number(str);
}

//-------------------------------------------------------------------------------//

int Is_Var(char **str)
{
    if (**str == 'x')
    {
        (*str)++;
        return Var_X;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Number(char **str)
{
    int value = 0;
    int len = 0;

    printf("%s\n", *str);

    sscanf(*str, "%d%n", &value, &len);
    assert(len);

    *str += len;

    return New_Num(value);
}

//-------------------------------------------------------------------------------//