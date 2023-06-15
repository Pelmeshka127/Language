#ifndef BACK_END_H_
#define BACK_END_H_

#include "../Tree/tree.h"

struct var_array {
    char *name;
    int id;
};

struct name_table {
    int size;
    int capacity;
    var_array *array;
};

int Name_Table_Ctor(name_table *const table, int capacity);

int Name_Table_Analyz(tree_node *const node, name_table *const table);

int Name_Table_Dtor(name_table *const table);

int Is_Uninitialised(tree_node *const node, name_table *const table);

int Make_Asm_File(FILE * dst_file, tree_node *const node, name_table *const table);

int Parse_Node(FILE *dst_file, tree_node *const node, name_table *const table);

int Parse_Label(FILE *dst_file, tree_node *const node, name_table *const table, int operation, char *oper);

int Parse_Function(FILE *dst_file, tree_node *const node, name_table *const table);

#endif