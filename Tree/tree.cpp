#include "tree.h"
#include "dump.h"
#include "../Front_End/lexer.h"
#include "../Front_End/frontend.h"
#include "../Back_End/back_end.h"

//-------------------------------------------------------------------------------//

FILE * log_file = nullptr;

//-------------------------------------------------------------------------------//

int Tree_Ctor(tree_s * const my_tree)
{
    my_tree->size = 0;

    my_tree->root = nullptr;

    return No_Error;
}
 
//-------------------------------------------------------------------------------//

tree_node * Tree_New_Node(type_t type, data_t data, char* name, tree_node *left, tree_node *right)
{
    tree_node * inserting_node = (tree_node *)calloc (1, sizeof(tree_node));
    if (inserting_node == nullptr)
    {
        fprintf(stderr, "Failed allocation new node in function %s\n", __PRETTY_FUNCTION__);
        return nullptr;
    }

    inserting_node->type  = type;
    inserting_node->data  = data;

    if (name)
    {
       inserting_node->name = (char *)calloc (Max_Size, sizeof(char));
       strncpy(inserting_node->name, name, Max_Size); 
    }

    inserting_node->left  = left;
    inserting_node->right = right;

    return inserting_node;
}

//-------------------------------------------------------------------------------//

int Tree_Get_Size(tree_s * const my_tree, tree_node * const node)
{
    if (my_tree->root == nullptr)
        return 0;

    if (node == my_tree->root)
        my_tree->size = 0;

    my_tree->size++;

    if (node->left)
        Tree_Get_Size(my_tree, node->left);

    if (node->right)
        Tree_Get_Size(my_tree, node->right);

    return my_tree->size;
}

//-------------------------------------------------------------------------------//

int Tree_Download(tree_s * const my_tree, char * file_name)
{
    Text_Info onegin = {};

    FILE * input_file  = fopen(file_name, "r");
    if (input_file == nullptr)
    {
        fprintf(stderr, "Failed reading file with source tree in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    if (Onegin_Read(&onegin, input_file) != No_Error)
    {
        fprintf(stderr, "Failed parsering in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    if (fclose(input_file) == EOF)
    {
        fprintf(stderr, "Failed clothing the source file in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    token_s tokens = {};

    Tokenizer(&tokens, &onegin);

    for (int i = 0; i < tokens.capacity; i++)
        printf("%s\n", tokens.array[i].name);

    my_tree->root = Get_General(&tokens); //Adding recursive decsent
    if (my_tree->root == nullptr)
    {
        fprintf(stderr, "Error in reading src file\n");
        Onegin_Dtor(&onegin);
        return File_Error;
    }

    name_table table = {};

    Name_Table_Ctor(&table, tokens.capacity);

    FILE *asm_file = fopen("asm.txt", "w");

    if (asm_file == nullptr)
    {
        fprintf(stderr, "Failed reading file with source tree in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    Create_Asm_File(asm_file, my_tree->root, &table);

    if (fclose(asm_file) == EOF)
    {
        fprintf(stderr, "Failed clothing the source file in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    Name_Table_Dtor(&table);
    Onegin_Dtor(&onegin);
    Token_Dtor(&tokens);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Get_Number_By_Operator(char * operation)
{
    if (strcmp("+", operation) == 0)
        return Op_Add;
    else if (strcmp("-", operation) == 0)
        return Op_Sub;
    else if (strcmp("*", operation) == 0)
        return Op_Mul;
    else if (strcmp("/", operation) == 0)
        return Op_Div;
    else if (strcmp("^", operation) == 0)
        return Op_Pow;
    else if (strcmp("ln", operation) == 0)
        return Op_Ln;
    else if (strcmp("sin", operation) == 0)
        return Op_Sin;
    else if (strcmp("cos", operation) == 0)
        return Op_Cos;
    else if (strcmp("exp", operation) == 0)
        return Op_Exp;
    else if (strcmp("=", operation) == 0)
        return Op_Asg;
    else if (strcmp("if", operation) == 0)
        return Op_If;
    else if (strcmp("<", operation) == 0)
        return Op_Below;
    else if (strcmp("<=", operation) == 0)
        return Op_Below_Eq;
    else if (strcmp(">", operation) == 0)
        return Op_Above;
    else if (strcmp(">=", operation) == 0)
        return Op_Above_Eq;
    else if (strcmp("==", operation) == 0)
        return Op_Eql;
    else if (strcmp("exit()", operation) == 0)
        return Op_End;
    else if (strcmp("elif", operation) == 0)
        return Op_Elif;
    else if (strcmp("else", operation) == 0)
        return Op_Else;
    else if (strcmp("!=", operation) == 0)
        return Op_Not_Eql;
    else if (strcmp("while", operation) == 0)
        return Op_While;
    else if (strcmp("input", operation) == 0)
        return Op_Input;
    else if (strcmp("print", operation) == 0)
        return Op_Print;

    else
        return Incorrect_Type;
}

//-------------------------------------------------------------------------------//

int Tree_Get_Operator_By_Number(int operation, char * oper_symbol, unsigned long len)
{
    if (operation == Op_Add)
        strncpy(oper_symbol, "+", len);
    else if (operation == Op_Sub)
        strncpy(oper_symbol, "-", len);
    else if (operation == Op_Mul)
        strncpy(oper_symbol, "*", len);
    else if (operation == Op_Div)
        strncpy(oper_symbol, "/", len);
    else if (operation == Op_Pow)
        strncpy(oper_symbol, "^", len);
    else if (operation == Op_Sin)
        strncpy(oper_symbol, "sin", len);
    else if (operation == Op_Cos)
        strncpy(oper_symbol, "cos", len);
    else if (operation == Op_Ln)
        strncpy(oper_symbol, "ln", len);
    else if (operation == Op_Exp)
        strncpy(oper_symbol, "exp", len);
    else if (operation == Op_Asg)
        strncpy(oper_symbol, "=", len);
    else if (operation == Op_If)
        strncpy(oper_symbol, "if", len);
    else if (operation == Op_And)
        strncpy(oper_symbol, "and", len);
    else if (operation == Op_Or)
        strncpy(oper_symbol, "or", len);
    else if (operation == Op_Below)
        strncpy(oper_symbol, "below", len);
    else if (operation == Op_Below_Eq)
        strncpy(oper_symbol, "below_equal", len);
    else if (operation == Op_Above)
        strncpy(oper_symbol, "above", len);
    else if (operation == Op_Above_Eq)
        strncpy(oper_symbol, "above_equal", len);
    else if (operation == Op_Eql)
        strncpy(oper_symbol, "==", len);
    else if (operation == Op_End)
        strncpy(oper_symbol, "exit()", len);
    else if (operation == Op_Elif)
        strncpy(oper_symbol, "elif", len);
    else if (operation == Op_Else)
        strncpy(oper_symbol, "else", len);
    else if (operation == Op_Not_Eql)
        strncpy(oper_symbol, "!=", len);
    else if (operation == Op_While)
        strncpy(oper_symbol, "while", len);
    else if (operation == Op_Input)
        strncpy(oper_symbol, "input", len);
    else if (operation == Op_Print)
        strncpy(oper_symbol, "print", len);

    else
    {
        //fprintf(stderr, "Incorrect type of oper %s in %s\n", operation, __PRETTY_FUNCTION__);
        return Incorrect_Node;
    }
    
    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Printer(tree_node * cur_node, const int print_type)
{   
    const char * file_name = nullptr;
    switch(print_type)
    {
        case Pre_Order:
        file_name = "trees/pre_order_tree.txt";
        break;

        case In_Order:
        file_name = "trees/in_order_tree.txt";
        break;

        case Post_Order:
        file_name = "trees/post_order_tree.txt";
        break;

        default:
        fprintf(stderr, "Incorrect type of tree print in function %s: %d\n", __PRETTY_FUNCTION__, print_type);
        return File_Error;
    }

    // FILE * dst_file = fopen(file_name, "w");
    // if (dst_file == nullptr)
    // {
    //     fprintf(stderr, "Failed openning file with of tree printing in function %s\n", __PRETTY_FUNCTION__);
    //     return File_Error;
    // }   

    // switch(print_type)
    // {
    //     case Pre_Order:
    //     Tree_Print_Pre_Order(cur_node, dst_file);
    //     break;

    //     case In_Order:
    //     Tree_Print_In_Order(cur_node, dst_file);
    //     break;

    //     case Post_Order:
    //     Tree_Print_Post_Order(cur_node, dst_file);
    //     break;
    // }

    // fclose(dst_file);

    return No_Error;
}

// //-------------------------------------------------------------------------------//

// int Tree_Print_Pre_Order(tree_node * const cur_node, FILE * dst_file)
// {
//     if (cur_node == nullptr)
//         return No_Error;

//     fprintf(dst_file, " ( ");

//     if (cur_node->type == Num_Type)
//         fprintf(dst_file, "%d", cur_node->data);

//     else if (cur_node->type == Op_Type)
//     {
//         char oper_symbol[Oper_Len] = "";
//         Tree_Get_Operator_By_Number(cur_node->data, oper_symbol, Oper_Len);
//         fprintf(dst_file, "%s", oper_symbol);
//     }

//     else if (cur_node->type == Var_Type)
//         fprintf(dst_file, "%c", Tree_Get_Variable_By_Number(cur_node->data));

//     Tree_Print_Pre_Order(cur_node->left, dst_file);
//     Tree_Print_Pre_Order(cur_node->right, dst_file);

//     fprintf(dst_file, " ) ");

//     return No_Error;
// }

// //-------------------------------------------------------------------------------//

// int Tree_Print_In_Order(tree_node * const cur_node, FILE * dst_file)
// {
//     if (cur_node == nullptr)
//         return No_Error;

//     fprintf(dst_file, " ( ");    

//     Tree_Print_In_Order(cur_node->left, dst_file);    

//     if (cur_node->type == Num_Type)
//         fprintf(dst_file, "%d", cur_node->data);
        
//     else if (cur_node->type == Op_Type)
//     {   
//         char oper_symbol[Oper_Len] = "";
//         Tree_Get_Operator_By_Number(cur_node->data, oper_symbol, Oper_Len);
//         fprintf(dst_file, "%s", oper_symbol);
//     }

//     else if (cur_node->type == Var_Type)
//         fprintf(dst_file, "%c", Tree_Get_Variable_By_Number(cur_node->data));

//     Tree_Print_In_Order(cur_node->right, dst_file);

//     fprintf(dst_file, " ) ");
    
//     return No_Error;
// }

// //-------------------------------------------------------------------------------//

// int Tree_Print_Post_Order(tree_node * const cur_node, FILE * dst_file)
// {
//     if (cur_node == nullptr)
//         return No_Error;

//     fprintf(dst_file, " ( ");

//     Tree_Print_Post_Order(cur_node->left, dst_file);
//     Tree_Print_Post_Order(cur_node->right, dst_file);

//     if (cur_node->type == Num_Type)
//         fprintf(dst_file, "%d", cur_node->data);

//     else if (cur_node->type == Op_Type)
//     {
//         char oper_symbol[Oper_Len] = "";
//         Tree_Get_Operator_By_Number(cur_node->data, oper_symbol, Oper_Len);
//         fprintf(dst_file, "%s", oper_symbol);
//     }

//     else if (cur_node->type == Var_Type)
//         fprintf(dst_file, "%c", Tree_Get_Variable_By_Number(cur_node->data));

//     fprintf(dst_file, " ) ");

//     return No_Error;
// }

// //-------------------------------------------------------------------------------//

int Tree_Find_Variable_Node(tree_node * const cur_node)
{
    if (cur_node != nullptr && cur_node->type == Var_Type)
        return Var_Type;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Is_There_Variables(tree_node * const cur_node)
{
    if (Tree_Find_Variable_Node(cur_node))
        return Var_Type;

    if (cur_node->right == nullptr && cur_node->left == nullptr)
        return No_Error;
    
    if (cur_node->left && Tree_Is_There_Variables(cur_node->left))
        return Var_Type;

    if (cur_node->right && Tree_Is_There_Variables(cur_node->right))
        return Var_Type;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Clean(tree_node ** root)
{
    if (*root == nullptr)
        return No_Error;

    if ((*root)->left != nullptr)
        Tree_Clean(&(*root)->left);

    if ((*root)->right != nullptr)
        Tree_Clean(&(*root)->right);

    free(*root);

    *root = nullptr;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Dtor(tree_s * const my_tree)
{
    assert(my_tree);

    Tree_Clean(&my_tree->root);

    my_tree->root = nullptr;

    my_tree->size = 0;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Check_Cmdline_Arg(int args)
{
    if (args != 2)
    {
        fprintf(stderr, "Error: wrong number of command line args: %d\n", args);
        return Cmdline_Error;
    }
    return No_Error;
}

int Eval(tree_node * const cur_node)
{
    if (cur_node->type == Num_Type)
        return cur_node->data;

    switch (cur_node->data)
    {

#       define DEF_CMD(name, number, code)     \
            case number:                        \
            code                                

#       include "../Architecture/cmd.h"

#       undef DEF_CMD

        default:
            fprintf(stderr, "Incorrect node type %d\n", cur_node->type);
    }

    return Incorrect_Node;
}