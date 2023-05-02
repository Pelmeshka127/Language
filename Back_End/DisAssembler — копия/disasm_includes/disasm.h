#ifndef DISASM_H_
#define DISASM_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../Architecture/proc_config.h"
#include "../../Architecture/architecture.h"

struct disasm_info {
    cmd_info * cmd_info;
    code_t * code;
};

//---------------------------------------------------------------------------------------------//

/// @brief Function allocates dynamic memory for source code
/// @param disasm is ptr on struct disasm_info
/// @param src_file is ptr on the src file
/// @return Alloc_Err if there's error, No_Error if it's ok
int Disasm_Ctor(disasm_info * const disasm, FILE * src_file);

//---------------------------------------------------------------------------------------------//

/// @brief 
/// @param disasm is ptr on struct disasm_info
/// @param dst_file 
/// @return 
int Disasm_Compile(disasm_info * const disasm, FILE * dst_file);

//---------------------------------------------------------------------------------------------//

/// @brief 
/// @param disasm is ptr on struct disasm_info
/// @param dst_file 
/// @return 
int Disasm_Dtor(disasm_info * const disasm, FILE * dst_file);

//---------------------------------------------------------------------------------------------//

#endif