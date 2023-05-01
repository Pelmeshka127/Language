#ifndef DECSENT_H_
#define DECSENT_H_

#include "../Tree/tree.h"
#include "../Architecture/dsl.h"
#include "../Tree/dump.h"

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_General(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Add_Or_Sub(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Mul_Or_Div(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Power(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Petrovich(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Number(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Var(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Func(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
int Is_Func(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param var 
/// @return 
int Is_Var(char *var);

/// @brief 
/// @param str 
void Skip_Spaces(Text_Info *onegin);

tree_node * Get_Assignment(Text_Info *onegin);

tree_node * Get_If(Text_Info *onegin);

tree_node * Get_Type(Text_Info *onegin);

void Move_Pointers(Text_Info *onegin);

tree_node * Get_If_Body(Text_Info *onegin);

int Is_Digit(char *word);

tree_node * Get_Log_Operator(Text_Info *onegin);

int Is_Log_Operation(char *operation);

#endif