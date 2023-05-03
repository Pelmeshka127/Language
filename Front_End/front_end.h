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
/// @param onegin 
/// @return 
tree_node * Get_Type(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param onegin 
/// @param condition_word 
/// @return 
tree_node * Get_Condition(Text_Info *onegin, char *condition_word);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param onegin 
/// @return 
tree_node * Get_Body(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param onegin 
/// @return 
tree_node * Get_Assignment(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param onegin 
/// @param end_word 
/// @return 
tree_node * Get_Exit(Text_Info *onegin, char *end_word);

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
/// @param onegin 
/// @return 
tree_node * Get_Log_Operator(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param str 
/// @return 
tree_node * Get_Func(Text_Info *onegin);

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
int Is_Func(Text_Info *onegin);

//-------------------------------------------------------------------------------//

/// @brief 
/// @param var 
/// @return 
int Is_Var(char *var);

/// @brief 
/// @param str 
void Skip_Spaces(Text_Info *onegin);

/// @brief 
/// @param onegin 
void Move_Pointers(Text_Info *onegin);

/// @brief 
/// @param word 
/// @return 
int Is_Digit(char *word);

/// @brief 
/// @param operation 
/// @return 
int Is_Log_Operation(char *operation);

/// @brief 
/// @param word 
/// @return 
int Is_Key_Word(char *word);

tree_node * Get_While(Text_Info *onegin);

int Stricmp(const char * str1, const char * str2);

#endif