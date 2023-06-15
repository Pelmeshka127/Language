#include "back_end.h"

static int If_Count     = 1;
static int Elif_Count   = 1;
static int Else_Count   = 1;
static int While_Count  = 1;

//-------------------------------------------------------------------------------//

int Name_Table_Ctor(name_table *const table, int capacity)
{
    table->array = (var_array *)calloc (capacity, sizeof(var_array));
    if (table->array == nullptr)
    {
        fprintf(stderr, "Error in function %s; failed allocation of the memory\n", __PRETTY_FUNCTION__);
        return Bad_Alloc;
    }

    table->capacity = capacity;

    for (int index = 0; index < table->capacity; index++)
    {
        table->array[index].name = (char *)calloc (Max_Size, sizeof(char));
        if (table->array[index].name == nullptr)
        {
            fprintf(stderr, "Error: Failed allocation of the memory"
                            "for strings in name_table in function %s\n", __PRETTY_FUNCTION__);
            return Bad_Alloc;
        }
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Make_Asm_File(FILE * dst_file, tree_node *const node, name_table *const table)
{
    assert(node);
    assert(table);

    Parse_Node(dst_file, node, table);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Parse_Node(FILE *dst_file, tree_node *const node, name_table *const table)
{
    assert(table);

    if (!node)
        return No_Error;

    switch(node->type)
    {
        case Num_Type:
            fprintf(dst_file, "push %d\n", node->data);
            break;

#   define DEF_CMD(word, number, len, code)     \
        case number:                            \
        code;                                   \
        break;                                  \

        case Op_Type:
        {
            switch(node->data)
            {
                #include "../Architecture/new_cmd.h"
            }
            break;
        }

#   undef DEF_CMD

        case Var_Type:
            if (Is_Uninitialised(node, table) == Incorrect_Var)
            {
                return Incorrect_Var;
            }
            fprintf(dst_file, "push [%d]\n", Name_Table_Analyz(node, table));
            break;

        case Connect_Type:
            Parse_Node(dst_file, node->left, table);
            Parse_Node(dst_file, node->right, table);
            break;

        case Function_Type:
            Parse_Function(dst_file, node, table);
            break;

        default:
            fprintf(stderr, "Incorrect type %d in %s\n", node->type, __PRETTY_FUNCTION__);
            break;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Parse_Label(FILE *dst_file, tree_node *const node, name_table *const table, int operation, char *oper)
{
    assert(table);

    if (node->type == Connect_Type)
    {
        Parse_Label(dst_file, node->left, table, operation, node->name);
        return No_Error;
    }

    Parse_Node(dst_file, node->left, table);
    Parse_Node(dst_file, node->right, table);

    char *operation_string = nullptr;
    operation_string = (char *)calloc (Max_Size, sizeof(char));

    if (strcmp(oper, "if") == 0)
        strcpy(operation_string, "if");
    else if (strcmp(oper, "elif") == 0)
        strcpy(operation_string, "elif");
    else if (strcmp(oper, "horny") == 0)
        stpcpy(operation_string, "end_while");

    switch(node->data)
    {
        case Op_Above:
            fprintf(dst_file, "ja :%s_%d\n", operation_string, operation);
            break;
        case Op_Above_Eq:
            fprintf(dst_file, "jae :%s_%d\n", operation_string, operation);
            break;
        case Op_Below:
            fprintf(dst_file, "jb :%s_%d\n",  operation_string, operation);
            break;
        case Op_Below_Eq:
            fprintf(dst_file, "jbe :%s_%d\n", operation_string, operation);
            break;
        case Op_Eql:
            fprintf(dst_file, "je :%s_%d\n",  operation_string, operation);
            break;
        case Op_Not_Eql:
            fprintf(dst_file, "jne :%s_%d\n", operation_string, operation);
            break;
        default:
            break;
    }

    free(operation_string);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Parse_Function(FILE *dst_file, tree_node *const node, name_table *const table)
{
    assert(table);

    switch(node->data)
    {
        case Op_Dec_Func:
            fprintf(dst_file, "jump: jump_over_%s\n", node->name);
            fprintf(dst_file, "%s:\n", node->name);
            Parse_Node(dst_file, node->left, table);
            Parse_Node(dst_file, node->right, table);
            fprintf(dst_file, "jump_over_%s:\n", node->name);
            break;
        
        case Op_Func_Name:
            fprintf(dst_file, "call %s:\n", node->name);
            break;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Name_Table_Analyz(tree_node *const node, name_table *const table)
{
    assert(node);
    assert(table);

    int id = -1;

    for (int index = 0; index < table->capacity; index++)
    {
        if (strcmp(node->name, table->array[index].name) == 0)
        {
            id = table->array[index].id;
        }
    }

    return id;
}

//-------------------------------------------------------------------------------//

int Name_Table_Dtor(name_table *const table)
{
    assert(table);

    for (int i = 0; i < table->capacity; i++)
        free(table->array[i].name);

    free(table->array);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Is_Uninitialised(tree_node *const node, name_table *const table)
{
    assert(table);

    if (Name_Table_Analyz(node, table) == -1)
    {
        fprintf(stderr, "Unitialised variable %s\n", node->name);
        return Incorrect_Var;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//