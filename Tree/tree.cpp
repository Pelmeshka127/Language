#include "tree.h"
#include "dump.h"
#include "../Front_End/lexer.h"
#include "../Front_End/frontend.h"
#include "../Back_End/back_end.h"

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

int Tree_Print_Pre_Order(FILE *dst_file, tree_node *const node)
{
    fprintf(dst_file, " ( ");

    Tree_Print_Node(dst_file, node);


    if (node->left || node->right)
    {
        if (node->left)
            Tree_Print_Pre_Order(dst_file, node->left);

        else
            fprintf(dst_file, " ( EMPTY ) ");

        if (node->right)
            Tree_Print_Pre_Order(dst_file, node->right);

        else
            fprintf(dst_file, " ( EMPTY ) ");
    }
    
    fprintf(dst_file, " ) ");

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Print_Node(FILE *dst_file, tree_node *const node)
{
    switch (node->type)
    {
        case Num_Type:
            fprintf(dst_file, "%d", node->data);
            break;

        case Var_Type:
            fprintf(dst_file, "var:\"%s\"", node->name);
            break;

        case Function_Type:
        {
            switch (node->data)
            {
                case Op_Dec_Func:
                    fprintf(dst_file, "func:\"%s\"", node->name);
                    break;

                case Op_Func_Name:
                    fprintf(dst_file, "call:\"%s\"", node->name);
                    break;
            }
            break;
        }

        case Connect_Type:
            fprintf(dst_file, "CN");
            break;

        case Op_Type:
            Tree_Print_Operation(dst_file, node);
            break;

        default:
            break; 
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Print_Operation(FILE *dst_file, tree_node *const node)
{
    switch (node->data)
    {
#       define  DEF_CMD(language_name, number, len, code_word, code)    \
        case number:                                                    \
            fprintf(dst_file, code_word);                               \
            break;                                                      \

        #include "../Architecture/new_cmd.h"

#       undef   DEF_CMD

        default:
            fprintf(stderr, "WTF????\n");
            break;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Reader(Text_Info *const onegin, tree_node ** node)
{
    Skip_Spaces(onegin);
    int number = 0;
    int length = 0;
    char word[Max_Length] = "";

    if (**onegin->pointers == '(')
    {
        Move_Pointers(onegin);
        Skip_Spaces(onegin);

        if (sscanf(*onegin->pointers, "%d%n", &number, &length))
        {
            *node = New_Num(number);
            (*onegin->pointers) += length;
        }

        else if (sscanf(*onegin->pointers, "%s%n", word, &length))
        {
            if (!(strchr(word, ':')))
            {
                char symbol[Max_Length] = "";

                int type = Tree_Reader_Type(word);

                int operation = Tree_Reader_Operation(word, symbol);

                if (type == Connect_Type)
                    *node = New_Connect_Type(nullptr, nullptr);
                
                else if (type == Op_Type)
                    *node = Tree_New_Node(Op_Type, operation, symbol);

                else 
                    *node = nullptr;

                (*onegin->pointers) += length;
            }

            else
            {
                char *symbol = strchr(word, '\"');
                symbol++;
                symbol[strlen(symbol) - 1] = '\0';

                if (Strincmp(word, "func", 4) == 0)
                    *node    = Tree_New_Node(Function_Type, Op_Dec_Func, symbol);

                else if (Strincmp(word, "call", 4) == 0)
                    *node    = Tree_New_Node(Function_Type, Op_Func_Name, symbol);

                else if (Strincmp(word, "var", 3) == 0)
                    *node    = Tree_New_Node(Var_Type, 0, symbol);

                else
                    *node = nullptr;

                (*onegin->pointers) += length;
            }
            
        }
    }

    Skip_Spaces(onegin);

    if (**onegin->pointers == ')')
    {
        Move_Pointers(onegin);
        return No_Error;
    }

    if (**onegin->pointers == '(')
        Tree_Reader(onegin, &(*node)->left);

    Skip_Spaces(onegin);

    if (**onegin->pointers == ')')
    {
        Move_Pointers(onegin);
        return No_Error;
    }

    if (**onegin->pointers == '(')
        Tree_Reader(onegin, &(*node)->right);

    Skip_Spaces(onegin);

    if (**onegin->pointers == ')')
    {
        Move_Pointers(onegin);
        return No_Error;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Reader_Type(char *const word)
{
    assert(word);

    if (strcmp(word, "CN") == 0)
        return Connect_Type;

    else if (strcmp(word, "EMPTY") == 0)
        return No_Error;

    else
        return Op_Type;
}

//-------------------------------------------------------------------------------//

int Tree_Reader_Operation(char *const word, char * dst_word)
{   
    assert(word);

#   define  DEF_CMD(language_word, number, len, code_word, code)    \
    if (Stricmp(code_word, word) == 0)                              \
    {                                                               \
        strncpy(dst_word, #language_word, strlen(#language_word));  \
        return number;                                              \
    }          

    #include "../Architecture/new_cmd.h"

#   undef   DEF_CMD

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Front_End(tree_s * const my_tree, char * file_name)
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

    // for (int i = 0; i < tokens.capacity; i++)
    //     printf("%s\n", tokens.array[i].name);

    my_tree->root = Get_General(&tokens);
    if (my_tree->root == nullptr)
    {
        fprintf(stderr, "Error in reading src file\n");
        Onegin_Dtor(&onegin);
        return File_Error;
    }

    Onegin_Dtor(&onegin);
    Token_Dtor(&tokens);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Make_Ast_Tree(tree_s *const my_tree)
{
    FILE *preorder = fopen("preorder.txt", "w");
    if (preorder == nullptr)
    {
        fprintf(stderr, "Failed reading file with source tree in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    Tree_Print_Pre_Order(preorder, my_tree->root);

    if (fclose(preorder) == EOF)
    {
        fprintf(stderr, "Failed clothing the source file in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Read_Ast_Tree(tree_s *const my_tree)
{
    FILE *src_file = fopen("preorder.txt", "r");
    if (src_file == nullptr)
    {
        fprintf(stderr, "Failed reading file with source tree in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    Text_Info parser = {};

    Onegin_Read(&parser, src_file);

    Tree_Reader(&parser, &my_tree->root);

    Tree_Dump_Node(my_tree->root);

    if (fclose(src_file) == EOF)
    {
        fprintf(stderr, "Failed clothing the source file in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    Onegin_Dtor(&parser);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Back_End(tree_s *const my_tree)
{
    name_table table = {};

    Name_Table_Ctor(&table, Tree_Get_Size(my_tree, my_tree->root));

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

    return No_Error;
}

//-------------------------------------------------------------------------------//