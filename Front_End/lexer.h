#ifndef LEXER_H_
#define LEXER_H_

#include "../Tree/tree.h"

//-------------------------------------------------------------------------------//

struct token_info {
    type_t type;
    data_t data;
    char *name;
};

//-------------------------------------------------------------------------------//

struct token_s {
    token_info *array;
    int capacity;
    int size;
};

//-------------------------------------------------------------------------------//

int Tokenizer(token_s *const tokens, Text_Info * const onegin);

//-------------------------------------------------------------------------------//

int Token_Dtor(token_s *tokens);

// //-------------------------------------------------------------------------------//

// void Skip_Spaces(Text_Info *const onegin);

// //-------------------------------------------------------------------------------//

// void Move_Pointers(Text_Info *const onegin);

// //-------------------------------------------------------------------------------//

int Token_Create_Number(token_s *const tokens, Text_Info *const onegin, int count);

//-------------------------------------------------------------------------------//

int Token_Create_Var(token_s *const tokens, Text_Info *const onegin, int count);

//-------------------------------------------------------------------------------//

int Strincmp(const char * str1, const char * str2, int const len);

//-------------------------------------------------------------------------------//

#endif