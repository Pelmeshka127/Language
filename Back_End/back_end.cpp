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
            Is_Uninitialised(node, table);
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

    if (strcmp(node->name, "hentai") == 0)
    {
        fprintf(dst_file, "call hentai\n");
        fprintf(dst_file, "hlt\n");
        fprintf(dst_file, "hentai:\n");
        Parse_Node(dst_file, node->right, table);
    }

    else
    {
        switch(node->data)
        {
            case Op_Dec_Func:
                fprintf(dst_file, "jump: jump_over_%s\n", node->name);
                fprintf(dst_file, "%s:\n", node->name);
                Parse_Declaration_Arguments(dst_file, node->left, table);
                Parse_Node(dst_file, node->right, table);
                fprintf(dst_file, "jump_over_%s:\n", node->name);
                break;
            
            case Op_Func_Name:
                Parse_Call_Arguments(dst_file, node->left, table);
                fprintf(dst_file, "call %s\n", node->name);
                break;

            default:
                break;
        }
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

int Add_Variable(tree_node *const node, name_table *const table)
{
    assert(table);

    strncpy(table->array[table->size].name, node->name, strlen(node->name));
    table->array[table->size].id = table->size;

    table->size++;

    return table->array[table->size - 1].id;
}

//-------------------------------------------------------------------------------//

int Parse_Call_Arguments(FILE *dst_file, tree_node *const node, name_table *table)
{
    assert(table);

    tree_node *cur_node = node;

    while (cur_node)
    {
        if (cur_node)
        {
            switch(cur_node->type)
            {
                case Connect_Type:
                    Parse_Call_Arguments(dst_file, cur_node->left, table);
                    break;
                
                case Num_Type:
                    Push_Pop_Arguments(dst_file, Cmd_Push, Not_Ram, cur_node->data); 
                    break;

                case Var_Type:
                    Push_Pop_Arguments(dst_file, Cmd_Push, Ram, Name_Table_Analyz(cur_node, table));
                    break;

                default:
                    break;
            }
        }
        cur_node = cur_node->right;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Push_Pop_Arguments(FILE *dst_file, int cmd, int destination, int value)
{
    if (cmd == Cmd_Push)
        fprintf(dst_file, "push ");

    else
        fprintf(dst_file, "pop ");

    if (destination == Ram)
        fprintf(dst_file, "[");

    fprintf(dst_file, "%d", value);

    if (destination == Ram)
        fprintf(dst_file, "]");
    
    fprintf(dst_file, "\n");

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Parse_Declaration_Arguments(FILE *dst_file, tree_node *const node, name_table *table)
{
    assert(table);

    tree_node *cur_node = node;

    int params_array[Max_Size] = {};

    int cur_param = 0;   

    while (cur_node)
    {
        if (cur_node->left)
        {
            switch(cur_node->left->type)
            {
                case Var_Type:
                {
                    int id = Add_Variable(cur_node->left, table);
                    params_array[cur_param] = id;
                    cur_param++;
                    break;
                }

                default:
                    break;
            }
        }

        cur_node = cur_node->right;
    }

    for (cur_param--; cur_param >= 0; cur_param--)
        Push_Pop_Arguments(dst_file, Cmd_Pop, Ram, params_array[cur_param]);

    return No_Error;  
}

//-------------------------------------------------------------------------------//