#ifndef FRONTEND_H_
#define FRONTEND_H_

#include <ctype.h>

#include "../Tree/tree.h"
#include "../Tree/dump.h"
#include "lexer.h"
#include "../Architecture/dsl.h"

//-------------------------------------------------------------------------------//

tree_node *Get_General(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Main(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Dec_Function(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Function_Call(token_s *const tokens, int parent_type);

//-------------------------------------------------------------------------------//

tree_node *Get_Function_Arguments(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Type(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Assignment(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_While(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_If(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Condition(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Init_Var(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Body(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Input(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Print(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Ret(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Exit(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Add_Sub(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Mul_Div(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Power(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Petrovich(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Math_Function(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Log_Operator(token_s *const tokens);

//-------------------------------------------------------------------------------//

tree_node *Get_Var(token_s *const tokens, int var_type);

//-------------------------------------------------------------------------------//

tree_node *Get_Number(token_s *const tokens);

//-------------------------------------------------------------------------------//

int Is_Log_Operation(char *word);

int Stricmp(const char * str1, const char * str2);


#endif