#include "back_end.h"

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

int Create_Asm_File(FILE *dst_file, tree_node *const node, name_table *const table)
{
    assert(table);

    if (!node)
        return No_Error;

    Create_Asm_File(dst_file, node->left, table);

    Create_Asm_File(dst_file, node->right, table);
    
    Print_Asm_Cmd(dst_file, node, table);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Print_Asm_Cmd(FILE *dst_file, tree_node *const node, name_table *const table)
{
    if (node->type == Num_Type)
    {
        fprintf(dst_file, "push %d\n", node->data);
    }

#   define DEF_CMD(word, number, len, code) \
        case number:                        \
        code;                               \
        break;                              \

    else if (node->type == Op_Type)
    {
        switch(node->data)
        {
            #include "../Architecture/new_cmd.h"
        }
    }

#   undef DEF_CMD

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
    for (int i = 0; i < table->capacity; i++)
        free(table->array[i].name);

    free(table->array);

    return No_Error;
}