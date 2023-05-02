#include <string.h>
#include <errno.h>


#include "../disasm_includes/disasm.h"

int main(void)
{
    disasm_info disasm = {};

    FILE * src_file = nullptr, * dst_file = nullptr;

    if ((src_file = fopen("../Assembler/prog.bin", "rb")) == nullptr)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return Src_File_Err;
    }

    if ((dst_file = fopen("disasm_file.txt", "w")) == nullptr)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return Src_File_Err;
    }

    if (Disasm_Ctor(&disasm, src_file) != No_Error)
        return Disasm_Compile_Error;
    
    Disasm_Compile(&disasm, dst_file);

    if (Disasm_Dtor(&disasm, dst_file) == Reading_File_Err)
        return Reading_File_Err;

    return 0;
}