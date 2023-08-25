#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../Architecture/architecture.h"
#include "../Parsering/parser.h"

const char StopWordName[Max_Length] = "STOP";

struct token_info {
    int     type;
    int     data;
    char    *name;
};

struct word_info {
    int     type;
    int     data;
    char    *word;
};

struct word_s {
    int         capacity;
    int         size;
    word_info   *array;
};

struct token_s {
    int         capacity;
    int         size;
    token_info  *array;
};

int     WordsCtor(word_s *const words, Text_Info *const onegin);

int     WordsProcessor(word_s *const words, Text_Info *const onegin);

int     MakeNumber(word_s *const words, Text_Info *const onegin);

int     MakeWord(word_s *const words, Text_Info *const onegin, const size_t len, const int num);

int     MakePunct(word_s *const words, Text_Info *const onegin);

int     MakeVar(word_s *const words, Text_Info *const onegin);

int     WordsDtor(word_s *const words);

bool    IsDigit(Text_Info *const onegin);

void    SkipSpaces(Text_Info *const onegin);

void    MovePointers(Text_Info *const onegin);

int     Strincmp(const char * str1, const char * str2, int const len);

int     TokenCtor(token_s *const tokens, word_s *const words);

int     Tokenizer(token_s *const tokens, word_s *const words);

int     TokenNumber(token_s *const tokens, word_s *const words);

int     TokenCompoundWordOperation(token_s *const tokens, word_s *const words);

int     TokenTrippleWord(token_s *const tokens, word_s *const words);

int     TokenDoubleWord(token_s *const tokens, word_s *const words);

int     TokenSimple(token_s *const tokens, word_s *const words, const int type);

int     TokenDtor(token_s *const tokens);

#endif