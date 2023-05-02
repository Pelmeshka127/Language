#include "../disasm_includes/disasm.h"

/// @brief 
/// @param disasm 
/// @param dst_file 
/// @param ip 
/// @param number 
static void Get_Arg(disasm_info * const disasm, FILE * dst_file, int * ip, int number);

//---------------------------------------------------------------------------------------------//

int Disasm_Ctor(disasm_info * const disasm, FILE * src_file)
{
    assert(disasm);

    disasm->cmd_info = (cmd_info *)calloc (1, sizeof(cmd_info));

    fread(disasm->cmd_info, 1, sizeof(cmd_info), src_file);
    if (disasm->cmd_info->CP != Def_CP)
        return Src_File_Err;

    disasm->code = (code_t *)calloc (disasm->cmd_info->file_size, sizeof(code_t));
    if (disasm->code == nullptr)
        return Alloc_Err;

    fread(disasm->code, sizeof(code_t), disasm->cmd_info->file_size, src_file);

    return No_Error;
}

//---------------------------------------------------------------------------------------------//

#define DEF_CMD(name, number, arg, ...)                                 \
    case number:                                                        \
        if (arg == Digit){                                              \
            fprintf(dst_file, "%s ", #name);                            \
            Get_Arg(disasm, dst_file, &ip, number);}                    \
        else if (arg == Label) {                                        \
            fprintf(dst_file, "%s %d\n", #name, disasm->code[ip + 1]);  \
            ip += sizeof(int); }                                        \
        else if (arg == 0)                                              \
            fprintf(dst_file, "%s\n", #name);                           \
        break;

//---------------------------------------------------------------------------------------------//

int Disasm_Compile(disasm_info * const disasm, FILE * dst_file)
{
    assert(disasm);

    int ip = 0;
    while (ip < disasm->cmd_info->file_size)
    {
        switch (disasm->code[ip] & CMD_MASK)
        {
            #include "../../Architecture/cmd.h"

            default:
                fprintf(stderr, "Undefined arg in %d %d\n", ip, disasm->code[ip]);
        }   
        ip++;
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//

static void Get_Arg(disasm_info * const disasm, FILE * dst_file, int * ip, int number)
{
    assert(disasm);

    if (disasm->code[*ip] & ARG_RAM)
    {
        fprintf(dst_file, "[");
        if (disasm->code[*ip] & ARG_IMMED && disasm->code[*ip] & ARG_REG)
        {
            fprintf(dst_file, "%d+%cx]\n", disasm->code[*ip+1], disasm->code[*ip+1+sizeof(int)] + 96);
            *ip += 2 * sizeof(int);
        }
        else if (disasm->code[*ip] & ARG_IMMED)
        {
            fprintf(dst_file, "%d]\n", disasm->code[*ip+1]);
            *ip += sizeof(int);
        }
        else if (disasm->code[*ip] & ARG_REG)
        { 
           fprintf(dst_file, "%cx]\n", disasm->code[*ip+1] + 96);
           *ip += sizeof(int);
        }
    }

    else if (disasm->code[*ip] & ARG_IMMED)
    {
        fprintf(dst_file, "%d\n", disasm->code[*ip+1]);
        *ip += sizeof(int);
        if (disasm->code[*ip] & ARG_REG)
        {
            fprintf(dst_file, "+%cx\n", disasm->code[*ip+2] + 96);
            *ip += sizeof(int);
        }
    }

    else if (disasm->code[*ip] & ARG_REG)
    {
        fprintf(dst_file, "%cx\n", disasm->code[*ip+1] + 96);
        *ip += sizeof(int);
    }

    else
        fprintf(stderr, "Undefined arg %d\n", disasm->code[*ip]);
}

//---------------------------------------------------------------------------------------------//

int Disasm_Dtor(disasm_info * const disasm, FILE * dst_file)
{
    if (fclose(dst_file) == false)
        return Reading_File_Err;
    free(disasm->code);
    free(disasm->cmd_info);
    return No_Error;
}

//---------------------------------------------------------------------------------------------//