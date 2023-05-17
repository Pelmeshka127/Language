#ifndef CPU_H_
#define CPU_H_

#include "../Stack/includes/stack.h"
#include "../../Architecture/proc_config.h"
#include "../../Architecture/architecture.h"

//---------------------------------------------------------------------------------------------//

/// @brief This struct includes the information about our CPU
struct cpu_info {
    cmd_info * cmd_info;
    //size_t size;        // The size of commands in file
    size_t ip;             // Index of the code_arary
    int * registers;    // The arrray of registers
    int * RAM;          // The array of RAM 
    code_t * code;      // The array of code which includes commands
    Stack stack;        // The struct of Stack
    Stack ret_stack;    // The Stack for functions
};

//---------------------------------------------------------------------------------------------//

/// @brief Function constructs cpu_info struct with info from source file and Stack library
/// @param cpu is pts on cpu_info struct
/// @param src_file is ptr on source file
/// @return Alloc_Err if construction failed, No_Error if it's ok
int CPU_Ctor(cpu_info * const cpu, FILE * src_file);

//---------------------------------------------------------------------------------------------//

/// @brief Function compiles the array of commands
/// @param cpu is pts on cpu_info struct
/// @return CPU_Compile_Error if compiling failed, No_Error if it's ok
int CPU_Execute(cpu_info * const cpu);

//---------------------------------------------------------------------------------------------//

/// @brief Function closes files and cleans allocated memory
/// @param cpu is ptr on cpu_info struct
/// @param src_file is ptr on source file
void CPU_Dtor(cpu_info * const cpu, FILE * src_file);

//---------------------------------------------------------------------------------------------//

/// @brief Function works with the argument of push
/// @param cpu is ptr on cpu_info struct
/// @param cmd is the number of current command
/// @param ip index of the code_arary
/// @return the value of the argumnet
int Get_Push_Arg(cpu_info * const cpu, int cmd);

//---------------------------------------------------------------------------------------------//

/// @brief Function works with the argument of pop
/// @param cpu is ptr on cpu_info struct
/// @param cmd is the number of current command
/// @param arg is the current argument of stack
int * Get_Pop_Arg(cpu_info * const cpu, int cmd);

//---------------------------------------------------------------------------------------------//

/// @brief Function works with the argument of jump
/// @param cpu is ptr on cpu_info struct
/// @return the new value of cpu->ip
size_t Jump_Arg(cpu_info * const cpu);

#endif