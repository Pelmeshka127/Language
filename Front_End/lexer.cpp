#include "lexer.h"
#include "../Architecture/dsl.h"

#include <ctype.h>

//-------------------------------------------------------------------------------//

int Tokenizer(token_s *const tokens, Text_Info *const onegin)
{
    tokens->array = (token_info *)calloc (onegin->symbols_count + 1, sizeof(token_info));

    for (int i = 0; i < onegin->symbols_count; i++)
    {
        if (onegin->buffer[i] == '\0')
            onegin->buffer[i] = '\n';
    }

    while (true)
    {
        if (**onegin->pointers == '\0')
            break;

        if ((**onegin->pointers >= '0' && **onegin->pointers <= '9') || (**onegin->pointers == '-' && *(*onegin->pointers + 1) >= '0' && *(*onegin->pointers + 1) <= '9'))
        {
            tokens->array[tokens->capacity].name = (char *)calloc (Max_Size, sizeof(char));
            Token_Create_Number(tokens, onegin, tokens->capacity);
            tokens->capacity++;
        }

        else if (**onegin->pointers == ',')
        {
            tokens->array[tokens->capacity].name = (char *)calloc (2, sizeof(char));
            tokens->array[tokens->capacity].data = Connect_Type;
            tokens->array[tokens->capacity].type = Connect_Type;
            strncpy(tokens->array[tokens->capacity].name, ",", 2);
            tokens->capacity++;
            (*onegin->pointers)++;
        }

        else if (Strincmp(FUNC_DECLARATION, *onegin->pointers, strlen(FUNC_DECLARATION)) == 0)
        {
            tokens->array[tokens->capacity].name = (char *)calloc (Max_Size, sizeof(char));
            strncpy(tokens->array[tokens->capacity].name, *onegin->pointers, strlen(FUNC_DECLARATION));
            tokens->array[tokens->capacity].type = Function_Type;
            tokens->array[tokens->capacity].data = Op_Dec_Func;
            tokens->capacity++;
            (*onegin->pointers) += strlen(FUNC_DECLARATION);
        }

        else 
            #define DEF_CMD(word, number, len, code) \
            if (Strincmp(#word, *onegin->pointers, len) == 0)   \
            {                                                   \
                tokens->array[tokens->capacity].type = Op_Type;            \
                tokens->array[tokens->capacity].data = number;              \
                tokens->array[tokens->capacity].name = (char *)calloc (len + 1, sizeof(char)); \
                strncpy(tokens->array[tokens->capacity].name, *onegin->pointers, len);       \
                tokens->capacity++;                             \
                (*onegin->pointers) += len;                      \
            }

                #include "../Architecture/new_cmd.h"

            #undef DEF_CMD

        else if (**onegin->pointers == '(' || **onegin->pointers == ')')
        {
            tokens->array[tokens->capacity].type = Op_Type;
            tokens->array[tokens->capacity].name = (char *)calloc (2, sizeof(char));
            strncpy(tokens->array[tokens->capacity].name, *onegin->pointers, 1);
            if (**onegin->pointers == '(')
                tokens->array[tokens->capacity].data = Op_Left_Br;
            else 
                tokens->array[tokens->capacity].data = Op_Right_Br;
            (*onegin->pointers)++;
            tokens->capacity++;
        }


        if (isalpha(**onegin->pointers))
        {
            tokens->array[tokens->capacity].name = (char *)calloc (Max_Size, sizeof(char));
            Token_Create_Var(tokens, onegin, tokens->capacity);
            tokens->capacity++;
        }

        else if (**onegin->pointers == ',')
        {
            tokens->array[tokens->capacity].name = (char *)calloc (2, sizeof(char));
            strncpy(tokens->array[tokens->capacity].name, ",", 2);
            (*onegin->pointers)++;
            tokens->capacity++;
        }
        
        Skip_Spaces(onegin);
    }

    (*onegin->pointers) -= onegin->symbols_count; // !!!!!!

    return No_Error;
}

//-------------------------------------------------------------------------------//

void Skip_Spaces(Text_Info *const onegin)
{
    while (isspace(**onegin->pointers))
    {
        Move_Pointers(onegin);
    }
}

//-------------------------------------------------------------------------------//

void Move_Pointers(Text_Info *const onegin)
{
    (*onegin->pointers)++;
    //onegin->char_num++;
}

//-------------------------------------------------------------------------------//

int Token_Dtor(token_s *tokens)
{
    for (int i = 0; i < tokens->capacity; i++)
    {
        free(tokens->array[i].name);
    }

    free(tokens->array);

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Token_Create_Number(token_s *const tokens, Text_Info *const onegin, int count)
{
    assert(tokens);
    assert(onegin);

    tokens->array[count].type = Num_Type;
    char *word = tokens->array[count].name;

    if (**onegin->pointers == '-')
    {
        *word++ = **onegin->pointers;
        (*onegin->pointers)++;
    }

    while ((**onegin->pointers >= '0' && **onegin->pointers <= '9'))
    {
        *word++ = **onegin->pointers;
        (*onegin->pointers)++;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Token_Create_Var(token_s *const tokens, Text_Info *const onegin, int count)
{
    assert(tokens);
    assert(onegin);

    tokens->array[count].type = Var_Type;
    char* word = tokens->array[count].name;

    while (isalpha(**onegin->pointers) || isdigit(**onegin->pointers))
    {
        *word++ = **onegin->pointers;
        (*onegin->pointers)++;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Strincmp(const char * str1, const char * str2, int const len)
{
    assert(str1);
    assert(str2);

    int i = 1;

    while(tolower(*str1) == tolower(*str2))
    {
        if (i == len)
            return 0;

        if (*str1 == '\0')
            return 0;

        str1++;
        str2++;
        i++;
    }
    return (int) (str1 - str2);
}

//-------------------------------------------------------------------------------//