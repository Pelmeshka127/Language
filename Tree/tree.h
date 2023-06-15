#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include "treeconfig.h"
#include "../Parsering/parser.h"
#include "../Architecture/architecture.h"

struct tree_node {
    type_t type;
    data_t data;
    char *name;
    tree_node * left;
    tree_node * right;
};

struct tree_s {
    tree_node * root;
    int size;               //number of the uneempty nodes
};

//-------------------------------------------------------------------------------//

/// @brief Function allocates memory for the root of the tree
/// @param my_tree is ptr on the tree struct
/// @param root is ptr on the tree's root
/// @param word is value of the root
/// @return nullptr is there's bad alloc, root if it's ok
int Tree_Ctor(tree_s * const my_tree);

//-------------------------------------------------------------------------------//

/// @brief Function allocates memory for the new node and inserts value to this node
/// @param my_tree is ptr on the tree struct
/// @param inserting_node is ptr on the inserting node
/// @param word is value of the node
/// @return nullptr if there's error, inserting_node if it's ok
tree_node * Tree_New_Node(type_t type, data_t data, char *name = nullptr, tree_node *left = nullptr, tree_node *right = nullptr);

//-------------------------------------------------------------------------------//

/// @brief Function finds out the size of the tree
/// @param my_tree is ptr on the tree struct
/// @return counts of the nodes in the tree
int Tree_Get_Size(tree_s * const my_tree, tree_node * const node);

//-------------------------------------------------------------------------------//

/// @brief Function makes tree from source file
/// @param my_tree is ptr on the tree struct
/// @return File_Error if creating tree failed, No_Error if it's ok
int Tree_Download(tree_s * const my_tree, char * file_name);

//-------------------------------------------------------------------------------//

int Tree_Print_Pre_Order(FILE * dst_file, tree_node *const cur_node);

/// @brief Function find out if the node is variable node
/// @param cur_node is ptr on the current node
/// @return Var_Type if it's variable, No_Error if it isn't
int Tree_Find_Variable_Node(tree_node * const cur_node);

//-------------------------------------------------------------------------------//

/// @brief Function looks after variables in the tree
/// @param cur_node is ptr on the current node
/// @return Var_Type if there's variable in the node, No_Error if there isn't variables
int Tree_Is_There_Variables(tree_node * const cur_node);

//-------------------------------------------------------------------------------//

/// @brief Fuction cleans all of the nodes
/// @param root is ptr on the root of the tree
/// @return No_Error
int Tree_Clean(tree_node ** root);

//-------------------------------------------------------------------------------//

/// @brief Function destroys tree
/// @param my_tree is ptr on the tree struct
/// @return No_Error
int Tree_Dtor(tree_s * const my_tree);

//-------------------------------------------------------------------------------//

int Check_Cmdline_Arg(int args);

int Eval(tree_node * const cur_node);

#endif