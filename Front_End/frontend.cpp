#include "frontend.h"

//-------------------------------------------------------------------------------//

tree_node *Get_General(token_s *const tokens)
{
    assert(tokens);

    tree_node *node = Get_Main(tokens);

    if (tokens->size != tokens->capacity)
    {
        fprintf(stderr, "Failed making tree because size %d!= capacity %d in %s; %d\n",
                tokens->size, tokens->capacity, __PRETTY_FUNCTION__, __LINE__);
        return nullptr;
    }

    Tree_Dump_Node(node);

    return node;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Main(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Function_Type) && TOKEN_DATA == Op_Dec_Func)
    {
        char symbol[Max_Size] = "";

        tokens->size++;

        if (strcmp(MAIN_FUNCTION, tokens->array[tokens->size].name) != 0)
        {
            return nullptr;
        }

        node_1 = Tree_New_Node(Function_Type, Op_Func_Name, tokens->array[tokens->size].name);

        strncpy(symbol, node_1->name, strlen(node_1->name));

        tokens->size++;

        if (strcmp(tokens->array[tokens->size].name, "im") != 0 || strcmp(tokens->array[tokens->size + 1].name, "stuck") != 0)
        {
            return nullptr;
        }

        tokens->size += 2;

        tree_node *node_2 = Get_Body(tokens);

        node_1 = Tree_New_Node(Function_Type, Op_Dec_Func, symbol, nullptr, node_2);

        node_1 = New_Connect_Type(node_1, Get_Dec_Function(tokens));
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Dec_Function(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Function_Type) && TOKEN_DATA == Op_Dec_Func)
    {
        char symbol[Max_Size] = "";

        tokens->size++;

        node_1 = Tree_New_Node(Function_Type, Op_Func_Name, tokens->array[tokens->size].name);

        strncpy(symbol, node_1->name, strlen(node_1->name));

        tokens->size++;

        if (strcmp(tokens->array[tokens->size].name, "im") == 0)
        {
            tokens->size++;
            node_1 = Get_Function_Arguments(tokens);
        }
        
        else
        {
            fprintf(stderr, "No im stuck!!\n");
            return nullptr;
        }

        tokens->size++;

        tree_node *node_2 = Get_Body(tokens);

        node_1 = Tree_New_Node(Function_Type, Op_Dec_Func, symbol, node_1, node_2);

        node_1 = New_Connect_Type(node_1, Get_Dec_Function(tokens));
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Function_Call(token_s *const tokens, int parent_type)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (strcmp(tokens->array[tokens->size + 1].name, "im") == 0)
    {
        char symbol[Max_Size] = "";

        node_1 = Tree_New_Node(Function_Type, Op_Func_Name, tokens->array[tokens->size].name);

        strncpy(symbol, node_1->name, strlen(node_1->name));

        tokens->size++;

        if (strcmp(tokens->array[tokens->size].name, "im") == 0)
        {
            tokens->size++;
            node_1 = Get_Function_Arguments(tokens);
        }
        
        else
        {
            fprintf(stderr, "No im stuck!!\n");
            return nullptr;
        }

        node_1 = Tree_New_Node(Function_Type, Op_Func_Name, symbol, node_1, nullptr);

        tokens->size++;

        if (parent_type != Op_Asg)
            node_1 = New_Connect_Type(node_1, Get_Type(tokens));

    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Function_Arguments(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;
    tree_node *node_2 = nullptr;

    while (strcmp(tokens->array[tokens->size].name, "stuck") != 0)
    {
        node_1 = Get_Add_Sub(tokens);

        if (TOKEN_TYPE(Connect_Type))
        {
            tokens->size++;
            node_2 = Get_Function_Arguments(tokens);
        }       
    }

    if (node_1 == nullptr)
        return nullptr;
        
    return New_Connect_Type(node_1, node_2);
}

//-------------------------------------------------------------------------------//

tree_node *Get_Type(token_s *const tokens)
{
    assert(tokens);

    if (TOKEN_TYPE(Var_Type) && strcmp(tokens->array[tokens->size + 1].name, "im") == 0)
        return Get_Function_Call(tokens, Op_Func_Name);

    else if (TOKEN_TYPE(Var_Type))
        return Get_Assignment(tokens);
    
    else if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_While)
        return Get_While(tokens);

    else if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_If)
        return Get_If(tokens);

    else if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Input)
        return Get_Input(tokens);

    else if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Print)
        return Get_Print(tokens);

    else if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_End)
        return Get_Exit(tokens);

    else if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Init)
        return Get_Init_Var(tokens);

    else if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Ret)
        return Get_Ret(tokens);

    else
    {
        //fprintf(stderr, "Unknown token [%s] in >????\n", tokens->array[tokens->size].name);
        return nullptr;
    }
}

//-------------------------------------------------------------------------------//

tree_node *Get_Assignment(token_s *const tokens)
{
    assert(tokens);
    
    tree_node *node_1 = Get_Var(tokens, Default_Var);

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Asg)
    {   
        assert(node_1->type == Var_Type);

        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, 1);

        tokens->size++;

        tree_node *node_2 = nullptr;

        if (strcmp(tokens->array[tokens->size + 1].name, "im") == 0)
            node_2 = Get_Function_Call(tokens, Op_Asg);

        else
            node_2 = Get_Add_Sub(tokens);


        node_1 = ASSIGNMENT(node_1, node_2, symbol);

        node_1 = New_Connect_Type(node_1, Get_Type(tokens));
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_While(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_While)
    {
        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, strlen(tokens->array[tokens->size].name));

        tokens->size++;

        node_1 = Get_Condition(tokens);

        tree_node *node_2 = Get_Body(tokens);

        node_1 = WHILE_NODE(node_1, node_2, symbol);

        node_1 = New_Connect_Type(node_1, Get_Type(tokens));
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_If(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;
    tree_node *node_2 = nullptr;
    tree_node *node_continue = nullptr;

    char *condition_word = tokens->array[tokens->size].name;
    int condition_num    = tokens->array[tokens->size].data;

    tokens->size++;

    if (strcmp(condition_word, ELSE_WORD) != 0)
        node_1 = Get_Condition(tokens);

    node_2 = Get_Body(tokens);

    node_1 = New_Connect_Type(node_1, node_2);

    if (TOKEN_TYPE(Op_Type) && (TOKEN_DATA == Op_Elif || TOKEN_DATA == Op_Else))
    {
        node_continue = Get_If(tokens);
    }

    node_1 = Tree_New_Node(Op_Type, condition_num, condition_word, node_1, node_continue);

    if (strcmp(condition_word, IF_WORD) == 0)
        return New_Connect_Type(node_1, Get_Type(tokens));
    else 
        return node_1;

}

//-------------------------------------------------------------------------------//

tree_node *Get_Condition(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Left_Br)
    {
        tokens->size++;

        node_1 = Get_Add_Sub(tokens);

        assert(TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Right_Br);

        tokens->size++;
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Body(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Start_Sex)
    {
        tokens->size++;

        node_1 = Get_Type(tokens);

        assert(TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_End_Sex);

        tokens->size++;
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Init_Var(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Init)
    {
        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, strlen(tokens->array[tokens->size].name));

        tokens->size++;

        node_1 = Get_Var(tokens, Default_Var);

        assert(TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Asg);

        tokens->size++;

        tree_node *node_2 = Get_Add_Sub(tokens);

        node_1 = Tree_New_Node(Op_Type, Op_Init, symbol, node_1, node_2);

        node_1 = New_Connect_Type(node_1, Get_Type(tokens));
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Input(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Input)
    {
        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, strlen(tokens->array[tokens->size].name));

        tokens->size++;

        node_1 = Get_Petrovich(tokens);

        node_1 = Tree_New_Node(Op_Type, Op_Input, symbol, node_1, nullptr);

        node_1 = New_Connect_Type(node_1, Get_Type(tokens));
    }

    return node_1;
}  

//-------------------------------------------------------------------------------//

tree_node *Get_Print(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1= nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Print)
    {
        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, strlen(tokens->array[tokens->size].name));
        
        tokens->size++;

        node_1 = Get_Petrovich(tokens);

        node_1 = Tree_New_Node(Op_Type, Op_Print, symbol, node_1, nullptr);

        node_1 = New_Connect_Type(node_1, Get_Type(tokens));
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Ret(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Ret)
    {
        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, strlen(tokens->array[tokens->size].name));

        tokens->size++;

        tree_node *node_2 = Get_Add_Sub(tokens);

        node_1 = Tree_New_Node(Op_Type, Op_Ret, symbol, node_2, nullptr);

        node_1 = New_Connect_Type(node_1, Get_Type(tokens));
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Exit(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_End)
    {
        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, strlen(tokens->array[tokens->size].name));

        tokens->size++;

        node_1 = Tree_New_Node(Op_Type, Op_End, symbol, nullptr, nullptr);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Add_Sub(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = Get_Mul_Div(tokens);

    while (TOKEN_TYPE(Op_Type) && (TOKEN_DATA == Op_Add || TOKEN_DATA == Op_Sub))
    {
        int operation = TOKEN_DATA;

        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, 1);

        tokens->size++;

        tree_node *node_2 = Get_Mul_Div(tokens);

        if (operation == Op_Add)
            node_1 = ADD(node_1, node_2, symbol);

        else 
            node_1 = SUB(node_1, node_2, symbol);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Mul_Div(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = Get_Power(tokens);

    while (TOKEN_TYPE(Op_Type) && (TOKEN_DATA == Op_Mul || TOKEN_DATA == Op_Div))
    {
        int operation = TOKEN_DATA;

        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, 1);

        tokens->size++;

        tree_node *node_2 = Get_Power(tokens);

        if (operation == Op_Mul)
            node_1 = MUL(node_1, node_2, symbol);

        else
            node_1 = DIV(node_1, node_2, symbol);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Power(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = Get_Log_Operator(tokens);

    while (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Pow)
    {
        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, 1);

        tokens->size++;

        tree_node *node_2 = Get_Log_Operator(tokens);

        node_1 = POW(node_1, node_2, symbol);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Log_Operator(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = Get_Math_Function(tokens);

    char *operation = tokens->array[tokens->size].name;

    int op_num = Is_Log_Operation(operation);
    if (op_num)
    {
        tokens->size++;

        tree_node *node_2 = Get_Math_Function(tokens);

        node_1 = Tree_New_Node(Op_Type, op_num, operation, node_1, node_2);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Math_Function(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = Get_Petrovich(tokens);

    if (TOKEN_TYPE(Op_Type) && (TOKEN_DATA == Op_Ln || TOKEN_DATA == Op_Sin || TOKEN_DATA == Op_Cos || TOKEN_DATA == Op_Exp || TOKEN_DATA == Op_Sqrt))
    {
        int operation = TOKEN_DATA;

        char symbol[Max_Size] = "";
        strncpy(symbol, tokens->array[tokens->size].name, strlen(tokens->array[tokens->size].name));

        tokens->size++;

        tree_node *node_2 = Get_Petrovich(tokens);

        if (operation == Op_Ln)
            node_1 = LN(node_2, symbol);
        else if (operation == Op_Sin)
            node_1 = SIN(node_2, symbol);
        else if (operation == Op_Cos)
            node_1 = COS(node_2, symbol);
        else if (operation == Op_Exp)
            node_1 = EXP(node_2, symbol);
        else if (operation == Op_Sqrt)
            node_1 = SQRT(node_2, symbol);
    }

    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Petrovich(token_s *const tokens)
{
    assert(tokens);

    tree_node *node_1 = nullptr;

    if (TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Left_Br)
    {
        tokens->size++;

        node_1 = Get_Add_Sub(tokens);

        assert(TOKEN_TYPE(Op_Type) && TOKEN_DATA == Op_Right_Br);

        tokens->size++;
    }

    else if (TOKEN_TYPE(Num_Type))
        node_1 = Get_Number(tokens);

    else if (TOKEN_TYPE(Var_Type) && strcmp(tokens->array[tokens->size + 1].name, "im") == 0)
    {
        //printf("Function as a operator\n");
        node_1 = Get_Function_Call(tokens, Op_Asg);
    }

    else if (TOKEN_TYPE(Var_Type))
    {
        //printf("%s is var\n", tokens->array[tokens->size].name);
        node_1 = Get_Var(tokens, Default_Var);
    }


    return node_1;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Number(token_s *const tokens)
{
    assert(tokens);

    int number = 0;

    sscanf(tokens->array[tokens->size].name, "%d", &number);

    tree_node *node = New_Num(number);

    tokens->size++;

    return node;
}

//-------------------------------------------------------------------------------//

tree_node *Get_Var(token_s *const tokens, int var_type)
{
    assert(tokens);

    char var[Max_Size] = "";

    sscanf(tokens->array[tokens->size].name, "%s", var);

    tree_node *node = New_Var(var, var_type);

    tokens->size++;

    return node;
}

//-------------------------------------------------------------------------------//

int Stricmp(const char * str1, const char * str2)
{
    assert(str1);
    assert(str2);

    while(tolower(*str1) == tolower(*str2))
    {
        if (*str1 == '\0')
            return 0;

        str1++;
        str2++;
    }
    return (int) (str1 - str2);
}

//-------------------------------------------------------------------------------//

int Is_Log_Operation(char *word)
{
    if (word == nullptr)
        return 0;

#   define LOG_OP(name, number)             \
    else if (Stricmp(#name, word) == 0)     \
        return number;

#   include "../Architecture/logoperators.h"

    else
        return 0;

#   undef LOG_OP

}

//-------------------------------------------------------------------------------//