#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>

#include "assembler.h"
#include "Architecture_Proc/proc_architecture.h"
#include "../Parsering/parser.h"

const int Label_Array_Multiplier = 2;

//---------------------------------------------------------------------------------------------//

/// @brief Function finds out labels in the src file and writes down it to the labels array
/// @param src_file is ptr on Text_Info struct 
/// @param asmbly is ptr in asm_file_info struct  
/// @param cur_num_str is a number of the current string
/// @param label_index is a index of the current label
/// @return Label_Error if Check_Label failed, No_Error if it's ok
static int Find_Labels(char * const cur_str, asm_file_info * const asmbly, int * cur_num_str, size_t * label_index);

//---------------------------------------------------------------------------------------------//

/// @brief Function compares labels with others who were already found
/// @param asmbly is ptr in asm_file_info struct 
/// @param cur_label is a string with the current label
/// @return Label_Error if there's the same label, No_Error if it's ok
static int Check_Label(asm_file_info * const asmbly, char * cur_label);

//---------------------------------------------------------------------------------------------//

/// @brief Function decreases the array of labels
/// @param asmbly is ptr in asm-file_info struct
/// @return Label_Error if there's error, No_Error if it's ok
static int Realloc_Label_Array(asm_file_info * const asmbly);

//---------------------------------------------------------------------------------------------//

/// @brief Function sets labels 
/// @param cur_str is a string with the current label
/// @param asmbly is ptr in asm_file_info struct 
/// @return the number of the label, 0 if label wasn't found
static int Set_Labels(char * cur_str, asm_file_info * const asmbly);

//---------------------------------------------------------------------------------------------//

/// @brief Funtion skips spaces until it finds label
/// @param str is ptr on the string
/// @return Label_Error if there's error, No_Error if isn't
static int Skip_Space_To_Label(char ** str);

//---------------------------------------------------------------------------------------------//

/// @brief Function finds out incorrect commands in source file
/// @param str is ptr on the string 
/// @param asmbly is ptr in asm_file_info struct                         
/// @param cur_num is a number of current string
static void Is_Space_Line(char ** str, asm_file_info * const asmbly, const int cur_num);

//---------------------------------------------------------------------------------------------//

/// @brief Function finds out extra arguments of commands
/// @param str is prt on the string
/// @param cur_num is a number of current string
/// @return Extra_Arg if there's error, No_Error if isn't               
static int Is_Extra_Arg(char ** str, int cur_num);

//---------------------------------------------------------------------------------------------//

/// @brief Function compares twp strings ignoring letter height
/// @param str1 is ptr on the first string
/// @param str2 is ptr on the second string                         
/// @return 0 if strings are equal, another digit if they're not
static int Stricmp(const char * str1, const char * str2);

//---------------------------------------------------------------------------------------------//\

/// @brief Function workes with the argument of the command
/// @param src_file is ptr on Text_Info struct 
/// @param asmbly is ptr in asm_file_info struct
/// @param cur_num_str is a number of current string
/// @param cmd the value of command
/// @return the code of argument or Undefined_Arg if argument is incorrect
static int Get_Arg(char * const cur_str, asm_file_info * const asmbly, int cur_num_str, int cmd);

//---------------------------------------------------------------------------------------------//

/// @brief Function workes with RAM's arguments
/// @param src_file is ptr on Text_Info struct 
/// @param asmbly is ptr in asm_file_info struct
/// @param cur_num_str is a number of current string
/// @param cmd the value of command
/// @return Undefined_Arg if argument is incorrect, No_Error if it's ok
static int Read_Ram_Arg(char * cur_str, asm_file_info * const asmbly, int cmd);

//---------------------------------------------------------------------------------------------//

/// @brief Function wokes with the registers and digits
/// @param src_file is ptr on Text_Info struct 
/// @param asmbly is ptr in asm_file_info struct
/// @param cur_num_str is a number of current string
/// @param cmd the value of command
/// @return Undefined_Arg if argument is incorrect, No_Error if it's ok
static int Read_Common_Arg(char * cur_str, asm_file_info * const asmbly, int cmd);

//---------------------------------------------------------------------------------------------//

/// @brief Functions checks if the argument is register
/// @param str is ptr on the string
/// @param asmbly is ptr in asm_file_info struct
/// @return The code of register or 0 if there's error
static int Is_Reg(char ** str, asm_file_info * const asmbly);

//---------------------------------------------------------------------------------------------//

int Asm_Ctor(FILE * input_file, Text_Info * const src_file, asm_file_info * const asmbly)
{
    assert(input_file);
    assert(src_file);

    if (Onegin_Read(src_file, input_file) != 0)
        return Reading_File_Err;
    
    if ((asmbly->cmd_info = (cmd_info *)calloc (1, sizeof(cmd_info))) == nullptr)
        return Alloc_Err;

    asmbly->cmd_info->CP = Def_CP;
    asmbly->cmd_info->file_size = 0;

    asmbly->error = No_Error;

    asmbly->code_arr = (code_t *)calloc (2 * src_file->lines_count, sizeof(int));
    if (asmbly->code_arr == nullptr)
        return Alloc_Err;
    
    asmbly->label_array_size = Def_Label_Size;

    asmbly->jumps_index = (int *)calloc (asmbly->label_array_size, sizeof(int));
    if (asmbly->jumps_index == nullptr)
        return Alloc_Err;

    asmbly->labels = (Labels *)calloc (asmbly->label_array_size , sizeof(Labels));
    if (asmbly->labels == nullptr)
        return Alloc_Err;

    for (size_t label = 0; label < asmbly->label_array_size; label++)
    {
        asmbly->labels[label].label_num = 0;
        asmbly->labels[label].label_line = nullptr;
    }

    return No_Error;
}

// TODO cmd - char, var. types - <done>

//---------------------------------------------------------------------------------------------//

#define DEF_CMD(name, number, arg, ...)                                                                         \
    else if (Stricmp(#name, cmd) == 0)                                                                          \
    {                                                                                                           \
        if (arg == Digit)                                                                                       \
        {                                                                                                       \
            (cur_str) += cmd_len;                                                                               \
            if (Get_Arg(cur_str, asmbly, cur_num_str, number) != No_Error)                                      \
            {                                                                                                   \
                fprintf(stderr, "Incorrect arg for command in line %d\n", cur_num_str + 1);                     \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
        else if (arg == Label)                                                                                  \
        {                                                                                                       \
            asmbly->jumps_index[jump_index++] = (int) asmbly->cmd_info->file_size;                              \
            asmbly->code_arr[asmbly->cmd_info->file_size] = 0;                                                  \
            int empty_label = 0;                                                                                \
            memcpy(&asmbly->code_arr[asmbly->cmd_info->file_size + 1], &empty_label, sizeof(int));              \
            asmbly->cmd_info->file_size += 1 + sizeof(int);                                                     \
        }                                                                                                       \
        else if (arg == 0)                                                                                      \
        {                                                                                                       \
            asmbly->code_arr[asmbly->cmd_info->file_size++] = number;                                           \
            if (Is_Extra_Arg(&cur_str, cmd_len) == Extra_Arg)                                                   \
            {                                                                                                   \
                asmbly->error = Extra_Arg;                                                                      \
                fprintf(stderr, "The string |%s| in line %d has an extra argument\n", cur_str, cur_num_str + 1);\
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
        cur_num_str++;                                                                                          \
    }
//---------------------------------------------------------------------------------------------//

int First_Asm_Compile(Text_Info * const src_file, asm_file_info * const asmbly)
{
    assert(src_file);
    assert(asmbly);

    int cur_num_str = 0;
    int jump_index  = 0;
    size_t label_index = 0;

    while (cur_num_str < (int) src_file->lines_count)
    {
        char cmd[Def_Cmd_Len] = { 0 };
        int cmd_len = 0;
        sscanf(src_file->pointers[cur_num_str], "%s%n", cmd, &cmd_len);
        char * cur_str = src_file->pointers[cur_num_str];
    
        if (strchr(cmd, ':') != nullptr)
        {
            if (Find_Labels(cur_str, asmbly, &cur_num_str, &label_index) == Label_Error)
            {
                fprintf(stderr, "Finding of labes failed\n");
                return Label_Error;
            }
        }

        #include "Architecture_Proc/proc_cmd.h"

        else
        {
            Is_Space_Line(&cur_str, asmbly, cur_num_str);
            if (asmbly->error == Undefined_Cmd)
            {
                fprintf(stderr, "Error: Incorrect command |%s| in input file in line %d\n", cur_str, cur_num_str + 1);
                return Undefined_Cmd;
                break;
            }
            else
                cur_num_str++;
        }
    }
    return No_Error;
}
#undef DEF_CMD
//---------------------------------------------------------------------------------------------//

// TODO label is not found - <done>

#define DEF_CMD(name, number, arg, ...)                                                                 \
    else if (Stricmp(#name, cmd) == 0)                                                                  \
    {                                                                                                   \
        if (arg == Label)                                                                               \
        {                                                                                               \
            asmbly->code_arr[asmbly->jumps_index[jump_index]] = number;                                 \
            (cur_str) += cmd_len;                                                                       \
            Skip_Space_To_Label(&cur_str);                                                              \
            int label_value = 0;                                                                        \
            if ((label_value = Set_Labels(cur_str, asmbly)) == 0)                                       \
            {                                                                                           \
                fprintf(stderr, "Label %s wasn't found\n", cur_str);                                    \
                return Label_Error;                                                                     \
            }                                                                                           \
            memcpy(&asmbly->code_arr[asmbly->jumps_index[jump_index++] + 1], &label_value, sizeof(int));\
        }                                                                                               \
        cur_num_str++;                                                                                  \
    }

//---------------------------------------------------------------------------------------------//

int Second_Asm_Compile(Text_Info * const src_file, asm_file_info * const asmbly)
{
    assert(src_file);
    assert(asmbly);

    int cur_num_str = 0;
    int jump_index = 0;

    while (cur_num_str < (int) src_file->lines_count)
    {
        char cmd[Def_Cmd_Len] = " ";
        int cmd_len = 0;
        sscanf(src_file->pointers[cur_num_str], "%s%n", cmd, &cmd_len);
        char * cur_str = src_file->pointers[cur_num_str];

        if (strchr(cmd, ':') != nullptr)
            cur_num_str++;

        #include "Architecture_Proc/proc_cmd.h"

        else
            cur_num_str++;
    }
    return No_Error;
}
#undef DEF_CMD

//---------------------------------------------------------------------------------------------//

static int Is_Reg(char ** str, asm_file_info * const asmbly)
{
    assert(str);
    assert(*str);
    assert(asmbly);

    char reg[Def_Cmd_Len] = { 0 };

    while (isspace(**str))
    {   
        (*str)++;

        if (isdigit(**str))
            return 0;
        if (isalpha(**str))
            break;
    }

    sscanf(*str, "%s", reg);

    if (strlen(*str) == 2 && (*str)[1] == 'x' && (*str)[0] >= 'a' && (*str)[0] <= 'z')
        return *str[0] - 'a' + 1;

    else
    {
        asmbly->error = Undefined_Arg;
        fprintf(stderr, "Incorrect reg in %s\n", *str);
        return 0;
    }
}

//---------------------------------------------------------------------------------------------//             

int Asm_Dtor(Text_Info * const src_file, asm_file_info * const asmbly, FILE * input_file)
{
    free(src_file->buffer);
    free(src_file->pointers);
    free(asmbly->code_arr);
    free(asmbly->labels);
    free(asmbly->jumps_index);
    free(asmbly->cmd_info);
    if (fclose(input_file) == false)
        return Reading_File_Err;

    return No_Error;
}

//---------------------------------------------------------------------------------------------//

int Check_Cmdline_Arg(int argc)
{
    if (argc != 3)
    {
        fprintf(stderr, "Error: wrong number of command line args: %d\n"
                "Expected %d\n", argc, 3);
        return Cmd_Line_Arg_Err;
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static void Is_Space_Line(char ** str, asm_file_info * const asmbly, int cur_num)
{
    assert(str);
    assert(*str);
    assert(cur_num);
    int length = (int) strlen(*str);

    int space_count = 0;
    while(**str != '\0')
    {
        if (isspace(**str))
            space_count++;
        (*str)++;
    }

    if (space_count != length)
        asmbly->error = Undefined_Cmd;
    else
        asmbly->error = No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Is_Extra_Arg(char ** str, int cmd_len)
{
    assert(str);
    assert(*str);

    (*str)+=cmd_len;

    while (**str != '\0')
    {
        if (!(isspace(**str)))
            return Extra_Arg;
        (*str)++;
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Stricmp(const char * str1, const char * str2)
{
    assert(str1);
    assert(str2);

    while(tolower(*str1) == tolower(*str2))
    {
        if (*str1 == '\0')
            return 0;

        str1++;
        str2++;
    }
    return (int) (str1 - str2);
}

//---------------------------------------------------------------------------------------------//

static int Find_Labels(char * const cur_str, asm_file_info * const asmbly, int * cur_num_str, size_t * label_index)
{
    assert(cur_str);
    assert(asmbly);

    if (*label_index >= asmbly->label_array_size - 1)
    {
        if (Realloc_Label_Array(asmbly) == Alloc_Err)
        {
            fprintf(stderr, "Recallocation of labels failed\n");
            return Label_Error;
        }
    }

    char * cur_label = cur_str;
    cur_label[strlen(cur_label) - 1] = '\0';

    if (Check_Label(asmbly, cur_label) == Label_Error)
    {
        fprintf(stderr, "The same label |%s| in line %d\n", cur_label, *cur_num_str + 1);
        return Label_Error;
    }

    asmbly->labels[*label_index].label_line = cur_label;
    asmbly->labels[*label_index].label_num  = (int) asmbly->cmd_info->file_size;
    *label_index += 1;
    *cur_num_str += 1;

    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Realloc_Label_Array(asm_file_info * const asmbly)
{
    assert(asmbly);

    asmbly->label_array_size *= Label_Array_Multiplier;

    asmbly->labels = (Labels *)realloc (asmbly->labels, asmbly->label_array_size * sizeof(Labels));
    if (asmbly->labels == nullptr)
        return Alloc_Err;
    
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Check_Label(asm_file_info * const asmbly, char * cur_label)
{
    assert(asmbly);
    assert(cur_label);

    for (size_t label = 0; label < asmbly->label_array_size; label++)
    {
        if (asmbly->labels[label].label_line != nullptr && Stricmp(cur_label, asmbly->labels[label].label_line) == 0)
        {
            asmbly->error = Label_Error;
            return Label_Error;
        }
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Set_Labels(char * cur_str, asm_file_info * const asmbly)
{
    assert(cur_str);
    assert(asmbly);

    for (size_t label = 0; label < asmbly->label_array_size; label++)
        if (asmbly->labels[label].label_line != nullptr && Stricmp(asmbly->labels[label].label_line, cur_str) == 0) //!!!!!!!
            return asmbly->labels[label].label_num;

    return 0;
}

//---------------------------------------------------------------------------------------------//

static int Skip_Space_To_Label(char ** str)
{
    assert(str);
    assert(*str);

    while (!(isalpha(**str)))
    {
        if (isdigit(**str))
            return Label_Error;
        (*str)++;
    }
    return No_Error;
}


//---------------------------------------------------------------------------------------------//

static int Get_Arg(char * const cur_str, asm_file_info * const asmbly, int cur_num_str, int cmd)
{
    assert(cur_str);
    assert(asmbly);

    if (strchr(cur_str, '[') != nullptr)
    {
        if (Read_Ram_Arg(cur_str, asmbly, cmd) == Undefined_Arg)
        {
            fprintf(stderr, "Error with making ram arg in |%s| in line %d\n", cur_str, cur_num_str + 1);
            return Undefined_Arg;
        }
    }

    else
    {
        if (Read_Common_Arg(cur_str, asmbly, cmd) == Undefined_Arg)
        {
            fprintf(stderr, "Error with making common arg in |%s| in line %d\n", cur_str, cur_num_str + 1);
            return Undefined_Arg;
        }
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Read_Ram_Arg(char * string, asm_file_info * const asmbly, int cmd)
{
    assert(string);
    assert(asmbly);

    cmd |= ARG_RAM;

    int immed_arg             =   0  ;
    char reg_arg[Def_Cmd_Len] = { 0 };
    char * cur_str            = strchr(string, '[') + 1;

    if (strchr(cur_str, '+') != nullptr)
    {
        sscanf(cur_str, "%d+%10s", &immed_arg, reg_arg);
        reg_arg[strlen(reg_arg) - 1] = '\0';
        cur_str = reg_arg;
        int reg = Is_Reg(&cur_str, asmbly);

        if (asmbly->error == Undefined_Arg)
            return Undefined_Arg;

        cmd = cmd | ARG_IMMED | ARG_REG;

        asmbly->code_arr[asmbly->cmd_info->file_size] = (code_t) cmd;
        memcpy(&asmbly->code_arr[asmbly->cmd_info->file_size + 1], &immed_arg, sizeof(int));
        memcpy(&asmbly->code_arr[asmbly->cmd_info->file_size + 1 + sizeof(int)], &reg, sizeof(int));

        asmbly->cmd_info->file_size += 1 + 2 * sizeof(int);
    }

    else
    {
        if (sscanf(cur_str, "%d", &immed_arg))
        {
            cmd |= ARG_IMMED;

            asmbly->code_arr[asmbly->cmd_info->file_size] = (code_t) cmd;
            memcpy(&asmbly->code_arr[asmbly->cmd_info->file_size + 1], &immed_arg, sizeof(int));
            
            asmbly->cmd_info->file_size += 1 + sizeof(int);
        }
        else if (sscanf(cur_str, "%s", reg_arg))
        {
            reg_arg[strlen(reg_arg) - 1] = '\0';
            cur_str = reg_arg;
            int reg = Is_Reg(&cur_str, asmbly);

            if (asmbly->error == Undefined_Arg)
                return Undefined_Arg;

            cmd |= ARG_REG;

            asmbly->code_arr[asmbly->cmd_info->file_size] = (code_t) cmd;
            memcpy(&asmbly->code_arr[asmbly->cmd_info->file_size + 1], &reg, sizeof(int));

            asmbly->cmd_info->file_size += 1 + sizeof(int);
        }
        else
            return Undefined_Arg;
    }
    
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static int Read_Common_Arg(char * string, asm_file_info * const asmbly, int cmd)
{
    assert(string);
    assert(asmbly);

    int immed_arg = 0;

    int reg_arg = Is_Reg(&string, asmbly);

    if (asmbly->error == Undefined_Arg)
        return Undefined_Arg;

    if (reg_arg != 0)
    {
        cmd |= ARG_REG;

        asmbly->code_arr[asmbly->cmd_info->file_size] = (code_t) cmd;
        memcpy(&asmbly->code_arr[asmbly->cmd_info->file_size + 1], &reg_arg, sizeof(int));

        asmbly->cmd_info->file_size += 1 + sizeof(int);

        return Reg;
    }

    sscanf(string, "%d", &immed_arg);

    cmd |= ARG_IMMED;

    asmbly->code_arr[asmbly->cmd_info->file_size] = (code_t) cmd;
    memcpy(&asmbly->code_arr[asmbly->cmd_info->file_size + 1], &immed_arg, sizeof(int));

    asmbly->cmd_info->file_size += 1 + sizeof(int);

    return Digit;
}

//---------------------------------------------------------------------------------------------//

#define DEF_CMD(name, number, arg, ...)                                                                                         \
    else if (Stricmp(#name, cmd) == 0)                                                                                          \
    {                                                                                                                           \
        if (arg == 0) {                                                                                                         \
            fprintf(listing, "\n%04zu      %04d    %-25s    %02d", cur_num_str, cmd_num, cur_str, asmbly->code_arr[cmd_num]);   \
            cmd_num++;}                                                                                                         \
        else if (arg == Label) {                                                                                                \
            fprintf(listing, "\n%04zu      %04d    %-25s    %02d    %02d",                                                      \
            cur_num_str, cmd_num, cur_str, asmbly->code_arr[cmd_num], Listing_Argument(asmbly, cmd_num));                       \
            cmd_num += 1 + sizeof(int);}                                                                                        \
        else if (arg == Digit) {                                                                                                \
            fprintf(listing, "\n%04zu      %04d    %-25s    %02d    %02d",                                                      \
            cur_num_str, cmd_num, cur_str, asmbly->code_arr[cmd_num], Listing_Argument(asmbly, cmd_num));                       \
            cmd_num += 1 + sizeof(int);}                                                                                        \
    }

//---------------------------------------------------------------------------------------------//

#ifdef LISTING

int List_Ctor()
{
    listing = fopen(list_file_name, "w");

    if (listing == nullptr)
        return Reading_File_Err;

    return No_Error;
}

//---------------------------------------------------------------------------------------------//

int Make_Listing(Text_Info * const list_file, asm_file_info * const asmbly)
{
    assert(list_file);
    assert(asmbly);

    size_t cur_num_str = 0;
    int cmd_num = 0;

    fprintf(listing, "Line     Addres   Text                         Code\n");

    while (cur_num_str < list_file->lines_count)
    {
        char cmd[Def_Cmd_Len] = { 0 };
        int cmd_len = 0;
        sscanf(list_file->pointers[cur_num_str], "%s%n", cmd, &cmd_len);
        char * cur_str = list_file->pointers[cur_num_str];
                
        if (strchr(cmd, ':') != nullptr)
            fprintf(listing, "\n%04zu      %04d    %-25s    %02d", cur_num_str, cmd_num, list_file->pointers[cur_num_str], cmd_num);

        #include "Architecture_Proc/proc_cmd.h"

        else
            fprintf(listing, "\n%04zu      %04d %-25s", cur_num_str, cmd_num, list_file->pointers[cur_num_str]);

        cur_num_str++;
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

int Listing_Argument(asm_file_info * const asmbly, int cmd_num)
{
    assert(asmbly);

    int argument = asmbly->code_arr[cmd_num + 1] + 
                   asmbly->code_arr[cmd_num + 2] * 256 +
                   asmbly->code_arr[cmd_num + 3] * 256 * 256 + 
                   asmbly->code_arr[cmd_num + 4] * 256 * 256 * 256;
        
    return argument;
}

//---------------------------------------------------------------------------------------------//

int List_Dtor(Text_Info * const list_info)
{
    if (fclose(listing) == false)
        return Reading_File_Err;
    
    free(list_info->buffer);
    free(list_info->pointers);

    return No_Error;
}

#endif

//---------------------------------------------------------------------------------------------//
