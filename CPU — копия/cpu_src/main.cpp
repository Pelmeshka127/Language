#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../cpu_includes/cpu.h"
#include "../Stack/includes/stack.h"
#include "../../Architecture/architecture.h"

int main(void)
{
    cpu_info cpu = {};

    FILE * bin_file = nullptr;

    cpu.stack = {};

    cpu.ret_stack = {};

    if ((bin_file = fopen("../Assembler/prog.bin", "rb")) == nullptr)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return Src_File_Err;
    }

    if (CPU_Ctor(&cpu, bin_file) != No_Error)
        return CPU_Compile_Error;

    if (CPU_Execute(&cpu) != No_Error)
        return CPU_Compile_Error;

    CPU_Dtor(&cpu, bin_file);

    return 0;
}