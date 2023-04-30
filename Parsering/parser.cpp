#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>

#include "parser.h"

//---------------------------------------------------------------------------------------------//

/// @brief Function gets the size of current file
/// @param file is ptr on current file
/// @return the size of the file
static int Get_File_Size(FILE * file, Text_Info * const Onegin);

//---------------------------------------------------------------------------------------------//

/// @brief Function gets the number of lines in the text
/// @param Onegin is ptr on Text_Info struct
/// @return the value of text's lines
static int Get_Line_Count(Text_Info * const Onegin);

//---------------------------------------------------------------------------------------------//

/// @brief Function makes string in the buffer by putting zero chat to the end of the line
/// @param Onegin is ptr on Text_Info struct
/// @return Bad_Alloc if there's allocation error, No_Error if it's ok
static int Make_Strings(Text_Info * const Onegin);

//---------------------------------------------------------------------------------------------//

int Onegin_Read(Text_Info * const Onegin, FILE * input_file)
{
    if (Get_File_Size(input_file, Onegin) == File_Error)
        return File_Error;

    Onegin->buffer = (char *) calloc( (unsigned int) Onegin->symbols_count + 1, sizeof(char));
    if (Onegin->buffer == NULL)
        return Bad_Alloc;

    if (fread(Onegin->buffer, sizeof(char), (unsigned int) Onegin->symbols_count, input_file) < (unsigned int) Onegin->lines_count)
        return File_Error;

    Onegin->lines_count = Get_Line_Count(Onegin) + 1;

    if (Make_Strings(Onegin) == Bad_Alloc)
        return Bad_Alloc;

    return No_Error;
}

//---------------------------------------------------------------------------------------------//

void Onegin_Print_To_File(Text_Info * const Onegin, FILE * fp)
{
    for (int cur_str = 0; cur_str < Onegin->lines_count; cur_str++)
        fprintf(fp, "%s\n", Onegin->pointers[cur_str]);
}

//---------------------------------------------------------------------------------------------//

void Onegin_Dtor(Text_Info * const Onegin)
{
    //free(Onegin->pointers);
    free(Onegin->buffer);
}

//---------------------------------------------------------------------------------------------//

static int Get_File_Size(FILE * fp, Text_Info * const Onegin)
{
    struct stat buf = {};

    if (fstat(fileno(fp), &buf) == -1)
        return File_Error;

    Onegin->symbols_count = (int) buf.st_size;

    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Get_Line_Count(Text_Info * const Onegin)
{
    assert(Onegin);

    int str_num = 0;

    for (int ch = 0; ch < Onegin->symbols_count; ch++)
        if (Onegin->buffer[ch] == '\n')
            str_num++;
            
    return str_num;
}

//---------------------------------------------------------------------------------------------//

static int Make_Strings(Text_Info * const Onegin)
{
    assert(Onegin);

    char ** Tmp_Mem = (char **)calloc ( (unsigned int) Onegin->symbols_count + 1, sizeof(char *));
    
    if (Tmp_Mem == NULL)
        return Bad_Alloc;

    Onegin->pointers = Tmp_Mem;

    *(Onegin->pointers) = Onegin->buffer;

    for (int ch = 0, cur_str = 1; ch < Onegin->symbols_count; ch++)
    {
        if (Onegin->buffer[ch] == '\n')
        {
            Onegin->buffer[ch] = '\0';
            Onegin->pointers[cur_str++] = &Onegin->buffer[ch + 1];
        }
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//