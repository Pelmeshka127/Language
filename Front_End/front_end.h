#ifndef DECSENT_H_
#define DECSENT_H_

#include "../Tree/tree.h"
#include "../Architecture/dsl.h"
#include "../Tree/dump.h"

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_General(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Add_Or_Sub(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Mul_Or_Div(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Power(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Petrovich(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Number(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Var(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Func(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
int Is_Func(char **str);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
int Is_Var(char **str);

/// @brief 
/// @param var 
/// @return 
int New_Is_Var(char *var);

/// @brief 
/// @param str 
void Skip_Spaces(char **str);

#endif