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

    Make_Asm_File(asm_file, my_tree->root, &table);

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

int Tree_Print_Pre_Order(FILE * dst_file, tree_node *const cur_node)
{
    if (cur_node == nullptr)
        return No_Error;

    fprintf(dst_file, " { ");

    switch(cur_node->type)
    {
        case Num_Type:
            fprintf(dst_file, "%d:%d", cur_node->type, cur_node->data);
            break;
        case Var_Type:
        case Function_Type:
        case Op_Type:
            fprintf(dst_file, "%d:%s", cur_node->type, cur_node->name);
            break;
        default:
            break;
    }

    Tree_Print_Pre_Order(dst_file, cur_node->left);
    Tree_Print_Pre_Order(dst_file, cur_node->right);

    fprintf(dst_file, " } ");

    return No_Error;
}

//-------------------------------------------------------------------------------//

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

//-------------------------------------------------------------------------------//

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

//-------------------------------------------------------------------------------//