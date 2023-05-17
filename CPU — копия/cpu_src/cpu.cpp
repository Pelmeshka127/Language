#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include <math.h>

#include "../cpu_includes/cpu.h"
#include "../Stack/includes/stack.h"

//---------------------------------------------------------------------------------------------//

#define DEF_CMD(name, number, arg, ...) \
    case number:                        \
    __VA_ARGS__                         \
    break;
 
//---------------------------------------------------------------------------------------------//

int CPU_Ctor(cpu_info * const cpu, FILE * src_file)
{
    assert(cpu);
    assert(src_file);

    cpu->cmd_info = (cmd_info *)calloc (1, sizeof(cmd_info));

    fread(cpu->cmd_info, sizeof(cmd_info), 1, src_file);
    if (cpu->cmd_info->CP != Def_CP)
        return CP_Error;

    cpu->code = (code_t *)calloc (cpu->cmd_info->file_size, sizeof(code_t));
    if (cpu->code == nullptr)
        return Alloc_Err;
    
    cpu->registers = (int *)calloc (Def_Reg_Size, sizeof(int));
    if (cpu->registers == nullptr)
        return Alloc_Err;

    cpu->RAM = (int *)calloc (Def_Ram_Size, sizeof(int));
    if (cpu->RAM == nullptr)
        return Alloc_Err;

    fread(cpu->code, sizeof(code_t), cpu->cmd_info->file_size, src_file);

    if (Stack_Ctor(&cpu->stack) == No_Error)
    {
		fprintf(stderr, "FAILED THE INITIALIZATION OF STACK\n");
		return Alloc_Err;
	}

    if (Stack_Ctor(&cpu->ret_stack) == No_Error)
    {
        fprintf(stderr, "FIALED THE INITIALIZATION OF RET_STACK\n");
        return Alloc_Err;
    }

    return No_Error;
}

//---------------------------------------------------------------------------------------------//

int CPU_Execute(cpu_info * const cpu)
{   
    assert(cpu);

    int mode = true;
    
    while (mode)
    {
        switch (cpu->code[cpu->ip] & CMD_MASK)
        {
            #include "../../Architecture/cmd.h"

            default:
            {
                fprintf(stderr, "Error, The command %d wasn't found\n", cpu->code[cpu->ip]);
                return CPU_Compile_Error;
            }
        }
        cpu->ip++;
    }
    return No_Error;
}

//---------------------------------------------------------------------------------------------//
// TODO reuse code -> get_arg
int Get_Push_Arg(cpu_info * const cpu, int cmd)
{
    assert(cpu);
    cpu->ip++;

    int arg       = 0;
    int immed_arg = 0;
    int reg_arg   = 0;

    if (cmd & ARG_IMMED)
    {
        memcpy(&immed_arg, &cpu->code[cpu->ip], sizeof(int));
        arg += immed_arg;
        cpu->ip += sizeof(int);
    }

    if (cmd & ARG_REG)
    {
        memcpy(&reg_arg, &cpu->code[cpu->ip], sizeof(int));
        if (reg_arg >= Def_Reg_Size)
        {
            fprintf(stderr, "Out of regs memeory in %d\n", reg_arg);
            return -1;
        }    
        arg += cpu->registers[reg_arg];
        cpu->ip += sizeof(int);
    }
                                    // TODO [imm + reg] -<done>
    if (cmd & ARG_RAM)
    {
        if (arg >= Def_Ram_Size)
        {
            fprintf(stderr, "Out of RAM in %d\n", arg);
            return -1;
        }
        arg = cpu->RAM[arg];
    }

    cpu->ip--;

    return arg;
}

//---------------------------------------------------------------------------------------------//

int * Get_Pop_Arg(cpu_info * const cpu, int cmd)
{
    assert(cpu);
    cpu->ip++;

    int immed_arg = 0;
    int reg_arg   = 0;

    if (cmd & ARG_RAM)
    {
        int Ram_index = 0;

        if (cmd & ARG_IMMED)
        {
            memcpy(&immed_arg, &cpu->code[cpu->ip], sizeof(int));
            Ram_index += immed_arg;
            cpu->ip += sizeof(int);
        }

        if (cmd & ARG_REG)
        {
            memcpy(&reg_arg, &cpu->code[cpu->ip], sizeof(int));
            Ram_index += cpu->registers[reg_arg];
            cpu->ip += sizeof(int);
        }
        
        if (Ram_index >= Def_Ram_Size)
        {
            fprintf(stderr, "Out of RAM in %d\n", Ram_index);
            return nullptr;
        }

        cpu->ip--;
        return &cpu->RAM[Ram_index];
    }

    if (cmd & ARG_REG)
    {
        memcpy(&reg_arg, &cpu->code[cpu->ip], sizeof(int));

        if (reg_arg >= Def_Reg_Size)
            fprintf(stderr, "Out of regs memeory in %d\n", reg_arg);

        cpu->ip += sizeof(int) - 1;

        return &cpu->registers[reg_arg];
    }

    else
        return 0;
}

//---------------------------------------------------------------------------------------------//

size_t Jump_Arg(cpu_info * const cpu)
{
    assert(cpu);

    size_t jump_arg = 0;

    memcpy(&jump_arg, &cpu->code[cpu->ip], sizeof(int));

    return jump_arg - 1;
}

//---------------------------------------------------------------------------------------------//

void CPU_Dtor(cpu_info * const cpu, FILE * src_file)
{
    assert(cpu);
    assert(src_file);

    Stack_Dtor(&cpu->stack);
    Stack_Dtor(&cpu->ret_stack);

    free(cpu->code);
    free(cpu->registers);
    free(cpu->RAM);
    free(cpu->cmd_info);
    fclose(src_file);
}

//---------------------------------------------------------------------------------------------//