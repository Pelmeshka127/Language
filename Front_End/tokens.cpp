#include "tokens.h"
#include "../Architecture/dsl.h"

#include <ctype.h>

//-------------------------------------------------------------------------------//

int Tokenizer(token_s *const tokens, Text_Info *const onegin)
{
    tokens->array = (token_info *)calloc (size_t(onegin->symbols_count + 1), sizeof(token_info));

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
            //printf("there is %s\n", FUNC_DECLARATION);
            tokens->array[tokens->capacity].name = (char *)calloc (Max_Size, sizeof(char));
            strncpy(tokens->array[tokens->capacity].name, *onegin->pointers, strlen(FUNC_DECLARATION));
            tokens->array[tokens->capacity].type = Function_Type;
            tokens->array[tokens->capacity].data = Op_Dec_Func;
            tokens->capacity++;
            (*onegin->pointers) += strlen(FUNC_DECLARATION);
        }

        else
        
#       define  DEF_CMD(language_word, number, len, code_word, code)                            \
            if (Strincmp(#language_word, *onegin->pointers, len) == 0)                          \
            {                                                                                   \
                tokens->array[tokens->capacity].type = Op_Type;                                 \
                tokens->array[tokens->capacity].data = number;                                  \
                tokens->array[tokens->capacity].name = (char *)calloc (len + 1, sizeof(char));  \
                strncpy(tokens->array[tokens->capacity].name, *onegin->pointers, len);          \
                tokens->capacity++;                                                             \
                (*onegin->pointers) += len;                                                     \
            }

                #include "../Architecture/new_cmd.h"

#       undef   DEF_CMD

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
            int symbol_idx = 0;

            tokens->array[tokens->capacity].name = (char *)calloc (Max_Size, sizeof(char));

            char *word = tokens->array[tokens->capacity].name;
            while (isalpha(**onegin->pointers) || isdigit(**onegin->pointers))
            {
                *word++ = **onegin->pointers;
                (*onegin->pointers)++;
            }
        
            tokens->capacity++;

            char *word_1 = (char *)calloc (Max_Size, sizeof(char));

            strncpy(word_1, tokens->array[tokens->capacity - 1].name, strlen(tokens->array[tokens->capacity - 1].name));

            Skip_Spaces(onegin);

            if (!(Token_Keyword(tokens, onegin, word_1, &symbol_idx)))
            {
                (*onegin->pointers)-= symbol_idx;
                tokens->array[tokens->capacity - 1].type = Var_Type;
            }

            free(word_1);

            Skip_Spaces(onegin);
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

int Token_Keyword(token_s *const tokens, Text_Info *const onegin, char *word_1, int *symbol_idx)
{
    if (**onegin->pointers != '\0')
    {
        char *word_new = (char *)calloc (Max_Size, sizeof(char));
        char *added_word = word_new;

        while(isalpha(**onegin->pointers) || isdigit(**onegin->pointers))
        {
            *added_word++ = **onegin->pointers;
            (*onegin->pointers)++;
            *symbol_idx += 1;
        }

        char *word_2 = (char *)calloc (Max_Size, sizeof(char));
        strncpy(word_2, word_new, strlen(word_new));

        char *word_3 = (char *)calloc (Max_Size * 2 + 1, sizeof(char));
        strncpy(word_3, word_1, strlen(word_1));
        strncat(word_3, "_", 1);
        strncat(word_3, word_2, strlen(word_2));

#       define  DEF_CMD(language_word, number, len, code_word, code)                            \
            if (Strincmp(#language_word, word_3, len) == 0)                                     \
            {                                                                                   \
                tokens->array[tokens->capacity - 1].type = Op_Type;                                 \
                tokens->array[tokens->capacity - 1].data = number;                                  \
                tokens->array[tokens->capacity - 1].name = (char *)calloc (len + 1, sizeof(char));  \
                strncpy(tokens->array[tokens->capacity - 1].name, word_3, len);          \
                /*printf("YES: %s and data is %d and type is %d\n", tokens->array[tokens->capacity - 1].name, tokens->array[tokens->capacity - 1].data, tokens->array[tokens->capacity - 1].type); */\
                free(word_2);   \
                free(word_3);   \
                return Op_Type; \
            }

                #include "../Architecture/new_cmd.h"

#       undef   DEF_CMD
    }

    return No_Error;
}


//сделать таблицу преобразования токенов(для склеивания)
//сделать энамы для раздельных слов и скливать их

//скласть токены позже