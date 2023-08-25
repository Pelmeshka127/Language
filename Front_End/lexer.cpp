#include "lexer.hpp"
#include "../Architecture/dsl.h"

#define Word_Type words->array[words->size].type
#define Word_Data words->array[words->size].data
#define Word_Name words->array[words->size].word

//-------------------------------------------------------------------------------//

int WordsCtor(word_s *const words, Text_Info *const onegin)
{
    words->capacity = onegin->symbols_count;

    words->array = (word_info *)calloc ((size_t) words->capacity, sizeof(words));
    if (words->array == nullptr)
    {
        fprintf(stderr, "Failed allocation for words array in %s\n", __func__);
        return Bad_Alloc;
    }

    for (int i = 0; i < onegin->symbols_count; i++)
    {
        if (onegin->buffer[i] == '\0')
            onegin->buffer[i] = '\n';
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int WordsProcessor(word_s *const words, Text_Info *const onegin)
{
    while (true)
    {
        if (**onegin->pointers == '\0')
            break;

        if (IsDigit(onegin))
        {
            // printf("- is %d\n", **onegin->pointers == '-');
            MakeNumber(words, onegin);
            continue;
        }

        else if (ispunct(**onegin->pointers))
        {
            MakePunct(words, onegin);
            continue;
        }

        else

#       define  DEF_CMD(language_word, number, len, code_word, code)     \
            if (Strincmp(#language_word, *onegin->pointers, len) == 0)    \
            {                                                              \
                MakeWord(words, onegin, len, number);                       \
                continue;                                                   \
            }

#       include "../Architecture/new_cmd.h"

#       undef   DEF_CMD

        if (isalpha(**onegin->pointers))
        {
            MakeVar(words, onegin);
        }

        SkipSpaces(onegin);
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int MakeNumber(word_s *const words, Text_Info *const onegin)
{
    Word_Type = Num_Type;

    Word_Name = (char *)calloc (Max_Length, sizeof(char));
    if (Word_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for %s in %s\n", *onegin->pointers, __func__);
        return Bad_Alloc;
    }

    char *word = Word_Name;

    if (**onegin->pointers == '-')
    {
        *word++ = **onegin->pointers;
        (*onegin->pointers)++;
    }

    while (isdigit(**onegin->pointers))
    {
        *word++ = **onegin->pointers;
        (*onegin->pointers)++;
    }

    words->size++;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int MakeWord(word_s *const words, Text_Info *const onegin, const size_t len, const int num)
{
    Word_Name = (char *)calloc (Max_Length, sizeof(char));
    if (Word_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for %s in %s\n", *onegin->pointers, __func__);
        return Bad_Alloc;
    }

    Word_Type = Op_Type;
    Word_Data = num;

    strncpy(words->array[words->size].word, *onegin->pointers, len);
    
    (*onegin->pointers) += len;

    if (Word_Data > 30)
        Word_Type = Double_Word_Type;

    words->size++;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int MakePunct(word_s *const words, Text_Info *const onegin)
{
    Word_Name = (char *)calloc (2, sizeof(char));
    if (Word_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for %s in %s\n", *onegin->pointers, __func__);
        return Bad_Alloc;
    }

    strncpy(Word_Name, *onegin->pointers, 1);

    Word_Type = Op_Type;

    if (**onegin->pointers == '(')
        Word_Data = Op_Left_Br;

    else if (**onegin->pointers == ')')
        Word_Data = Op_Right_Br;

    else if (**onegin->pointers == ',')
    {
        Word_Data = Op_Comma;
        Word_Type = Connect_Type;
    }

    else

#   define  DEF_CMD(language_word, number, len, code_word, code)    \
    if (strcmp(#language_word, Word_Name) == 0)                     \
        Word_Data = number;

#   include "../Architecture/new_cmd.h"

#   undef   DEF_CMD

    MovePointers(onegin);

    words->size++;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int MakeVar(word_s *const words, Text_Info *const onegin)
{
    Word_Name = (char *)calloc (Max_Length, sizeof(char));
    if (Word_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for %s in %s\n", *onegin->pointers, __func__);
        return Bad_Alloc;
    }

    char *word = Word_Name;

    while (isalpha(**onegin->pointers) || isdigit(**onegin->pointers))
    {
        *word++ = **onegin->pointers;
        MovePointers(onegin);
    }

    Word_Type = Var_Type;

    words->size++;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int WordsDtor(word_s *const words)
{
    for (int i = 0; i < words->size; i++)
        free(words->array[i].word);

    free(words->array);

    return No_Error;
}

//-------------------------------------------------------------------------------//

bool IsDigit(Text_Info *const onegin)
{
    return (isdigit(**onegin->pointers) || 
    ((**onegin->pointers == '-') && (isdigit(*(*onegin->pointers + 1))) ) );
}

//-------------------------------------------------------------------------------//

void SkipSpaces(Text_Info *const onegin)
{
    while (isspace(**onegin->pointers))
        MovePointers(onegin);
}

//-------------------------------------------------------------------------------//

void MovePointers(Text_Info *const onegin)
{
    (*onegin->pointers)++;
}

//-------------------------------------------------------------------------------//

#undef Word_Type
#undef Word_Data
#undef Word_Name

#define Token_Type tokens->array[tokens->capacity].type
#define Token_Data tokens->array[tokens->capacity].data
#define Token_Name tokens->array[tokens->capacity].name

//-------------------------------------------------------------------------------//
int TokenCtor(token_s *const tokens, word_s *const words)
{
    tokens->array = (token_info *)calloc ((size_t) words->size, sizeof(token_info));
    if (tokens->array == nullptr)
    {
        fprintf(stderr, "Failed allocation memory for token array in %s\n", __func__);
        return Bad_Alloc;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tokenizer(token_s *const tokens, word_s *const words)
{
    int size    = words->size;
    words->size = 0;

    while (words->size != size)
    {
        switch(words->array[words->size].type)
        {
            case Num_Type:
                TokenNumber(tokens, words);
                break;

            case Op_Type:
                TokenSimple(tokens, words, words->array[words->size].type);
                break;

            case Var_Type:
                TokenSimple(tokens, words, words->array[words->size].type);
                break;

            case Connect_Type:
                TokenSimple(tokens, words, words->array[words->size].type);
                break;

            case Double_Word_Type:
                TokenCompoundWordOperation(tokens, words);
                break;
                
            default:
            {
                fprintf(stderr, "Undefined type %d of %s\n", 
                        words->array[words->size].data, 
                        words->array[words->size].word);
                return Incorrect_Type;
            }
        }
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int TokenNumber(token_s *const tokens, word_s *const words)
{   
    Token_Type = Num_Type;

    int value = 0;
    
    sscanf(words->array[words->size].word, "%d", &value);

    Token_Data = value;

    size_t word_len = strlen(words->array[words->size].word);

    Token_Name = (char *)calloc (word_len + 1, sizeof(char));
    if (Token_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for token word in %s\n", __func__);
        return Bad_Alloc;
    }

    strncpy(Token_Name, words->array[words->size].word, word_len);

    tokens->capacity++;

    words->size++;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int TokenCompoundWordOperation(token_s *const tokens, word_s *const words)
{
    if (words->array[words->size].data == Nice)
    {
        if (words->array[words->size + 1].data == Small ||
            words->array[words->size + 1].data == Big)
        {
            TokenTrippleWord(tokens, words);
            tokens->capacity++;
            words->size += 3;
        }

        else
        {
            TokenDoubleWord(tokens, words);
            tokens->capacity++;
            words->size += 2;
        }
    }

    else
    {
        TokenDoubleWord(tokens, words);

        if (Token_Data == Op_Step_Brother)
        {
            Token_Type = Function_Type;
            Token_Data = Op_Dec_Func;
        }

        tokens->capacity++;
        words->size += 2;
    }

    return No_Error;
}

//-------------------------------------------------------------------------------//

int TokenTrippleWord(token_s *const tokens, word_s *const words)
{
    size_t  len1        = strlen(words->array[words->size].word),
            len2        = strlen(words->array[words->size].word),
            len3        = strlen(words->array[words->size].word),
            len_word    = len1 + len2 + len3 + 2;

    Token_Name = (char *)calloc (len_word + 1, sizeof(char));
    if (Token_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for token word in %s\n", __func__);
        return Bad_Alloc;
    }

    strncpy(Token_Name, words->array[words->size].word, len1);
    strncat(Token_Name, "_", 1);
    strncat(Token_Name, words->array[words->size + 1].word, len2);
    strncat(Token_Name, "_", 1);
    strncat(Token_Name, words->array[words->size + 2].word, len3);

    Token_Type = Op_Type;

    Token_Data =  words->array[words->size].data       +
                  words->array[words->size + 1].data   +
                  words->array[words->size + 2].data;
    return No_Error;
}

//-------------------------------------------------------------------------------//

int  TokenDoubleWord(token_s *const tokens, word_s *const words)
{
    size_t  len1        = strlen(words->array[words->size].word),
            len2        = strlen(words->array[words->size + 1].word),
            len_word    = len1 + len2 + 1;

    Token_Name = (char *)calloc (len_word + 1, sizeof(char));
    if (Token_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for token word in %s\n", __func__);
        return Bad_Alloc;
    }

    strncpy(Token_Name, words->array[words->size].word, len1);
    strncat(Token_Name, "_", 1);
    strncat(Token_Name, words->array[words->size + 1].word, len2);

    Token_Type = Op_Type;

    Token_Data =  words->array[words->size].data +
                  words->array[words->size + 1].data;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int TokenSimple(token_s *const tokens, word_s *const words, const int type)
{
    size_t len_word = strlen(words->array[words->size].word);

    Token_Name = (char *)calloc (len_word + 1, sizeof(char));
    if (Token_Name == nullptr)
    {
        fprintf(stderr, "Failed allocation for token word in %s\n", __func__);
        return Bad_Alloc;
    }

    strncpy(Token_Name, words->array[words->size].word, len_word);

    Token_Type = type;

    Token_Data = words->array[words->size].data;

    tokens->capacity++;

    words->size++;

    return No_Error;
}

//-------------------------------------------------------------------------------//

int TokenDtor(token_s *const tokens)
{
    for (int i = 0; i < tokens->capacity; i++)
        free(tokens->array[tokens->capacity].name);

    free(tokens->array);

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

#undef Token_Type
#undef Token_Data
#undef Token_Name