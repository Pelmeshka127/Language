#ifndef DUMP_H_
#define DUMP_H_

#include "tree.h"

#define TREE_DUMP

//-------------------------------------------------------------------------------//

/// @brief Function opens log_file
/// @return File_Error if there's error, No_Error if it's ok
int Open_Log_File();

//-------------------------------------------------------------------------------//

/// @brief Function closes log_file
/// @return File_Error if there's error, No_Error if it's ok
int Close_Log_File();

//-------------------------------------------------------------------------------//

/// @brief Function make picture of tree
/// @param my_tree is ptr on the tree struct
int Tree_Dump(tree_s * const my_tree);

//-------------------------------------------------------------------------------//

/// @brief Function draws nodes of the tree
/// @param root is ptr on the tree node
void Tree_Draw(tree_node * const node);

//-------------------------------------------------------------------------------//

void Tree_Dump_To_Tex(tree_s * const my_tree, char * png_name);

int Tree_Dump_Node(tree_node * const node);

#endif