#include "front_end.h"
#include <ctype.h>

//-------------------------------------------------------------------------------//

tree_node * Get_General(Text_Info * onegin)
{
    if (strcmp(onegin->pointers[onegin->lines_count - 1], "i'm_cumming") != 0)
    {
        fprintf(stderr, "No finish word((\n");
        return nullptr;
    }

    tree_node *node = Get_Type(onegin);

    // printf("%s\n", *onegin->pointers);
    // printf("%c\n", **onegin->pointers);

    assert(**onegin->pointers == '\0');

    assert(onegin->char_num == onegin->symbols_count);

    if (onegin->char_num != onegin->symbols_count)
    {
        fprintf(stderr, "Error, other values of char\n");
        return nullptr;
    }

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

    if (Is_Key_Word(var))
        return nullptr;

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

    return Get_Number(onegin);

    // if (Is_Digit(var))
    //     return Get_Number(onegin);

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

        node_1 = EQUAL(node_1, node_2);

        node_1 = New_Connect_Type(node_1, Get_Type(onegin));

        Tree_Dump_Node(node_1);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Condition(Text_Info * onegin, char *condition_word)
{
    tree_node *node_1 = nullptr;
    tree_node *node_2 = nullptr;

    if (strcmp(condition_word, "else") != 0)
    {
        while (**onegin->pointers != '(')
            Move_Pointers(onegin);
    
        Move_Pointers(onegin);

        node_1 = Get_Add_Or_Sub(onegin);
    }

    else
    {
        (*onegin->pointers) += strlen(condition_word);
        onegin->char_num += strlen(condition_word);
    }

    while (!(isalpha(**onegin->pointers)))
        Move_Pointers(onegin);

    node_2 = Get_Body(onegin);
    if (node_2 == nullptr)
        return nullptr;

    node_1 = Tree_New_Node(Op_Type, Tree_Get_Number_By_Operator(condition_word), condition_word, node_1, node_2);

    Tree_Dump_Node(node_1);

    Skip_Spaces(onegin);

    char word[Max_Size] = "";
    sscanf(*onegin->pointers, "%s", word);

    tree_node *node_continue = nullptr;

    if (strcmp(word, "elif") == 0 || strcmp(word, "else") == 0)
    {
        node_continue = Get_Condition(onegin, word);
        node_1 = New_Connect_Type(node_1, node_continue);
    }

    return New_Connect_Type(node_1, Get_Type(onegin));
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

    Skip_Spaces(onegin);

    char *tokens = *onegin->pointers;
    char word[Max_Size] = "";
    
    sscanf(tokens, "%s", word);

    if (strcmp(word, "if") == 0 && strchr(tokens, '(') != nullptr && strchr(tokens, ')') != nullptr)
    {
        return Get_Condition(onegin, word);
    }

    else if (strcmp(word, "i'm_cumming") == 0)
        return Get_Exit(onegin, word);

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
    if (strcmp("small_dick", operation) == 0)
        return Op_Less;
    else if (strcmp("nice_small_dick", operation) == 0)
        return Op_Less_Eq;
    else if (strcmp("big_dick", operation) == 0)
        return Op_More;
    else if (strcmp("nice_big_dick", operation) == 0)
        return Op_More_Eq;
    else if (strcmp("nice_dick", operation) == 0)
        return Op_ChecK;
    else if (strcmp("ass", operation) == 0)
        return Op_And;
    else if (strcmp("oral_sex", operation) == 0)
        return Op_Or;

    else
        return 0;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Exit(Text_Info *onegin, char *end_word)
{
    Skip_Spaces(onegin);

    (*onegin->pointers)+=strlen(end_word);
    onegin->char_num+=strlen(end_word);

    return Tree_New_Node(Op_Type, Op_End, "exit()", nullptr, nullptr);
}

//-------------------------------------------------------------------------------//

int Is_Key_Word(char *word)
{
    if (strcasecmp(word, "start_sex") == 0)
        return 1;
    else if (strcasecmp(word, "end_sex") == 0)
        return 1;

    else
        return 0;
}

//-------------------------------------------------------------------------------//

tree_node * Get_Body(Text_Info *onegin)
{   
    char word[Max_Size] = "";
    sscanf(*onegin->pointers, "%s", word);

    tree_node *node = nullptr;

    if (strcmp(word, "start_sex") == 0)
    {
        (*onegin->pointers) += strlen(word);
        onegin->char_num += strlen(word);

        Skip_Spaces(onegin);

        node = Get_Type(onegin);

        sscanf(*onegin->pointers, "%s", word);

        if (strcmp(word, "end_sex") == 0)
        {
            (*onegin->pointers) += strlen(word);
            onegin->char_num += strlen(word);
        }

        else
        {
            fprintf(stderr, "No end_sex!!!! in %s\n", *onegin->pointers);
            return nullptr;
        }
    }   

    else
        fprintf(stderr, "%s:It's not a body((( of condition\n", *onegin->pointers);

    return node;
}   

//-------------------------------------------------------------------------------//