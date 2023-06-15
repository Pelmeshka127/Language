#define PUSH_STACK(elem)  \
    Stack_Push(&cpu->stack, elem)

#define POP_STACK   \
    Stack_Pop(&cpu->stack)

#define PUSH_RET_STACK(elem)    \
    Stack_Push(&cpu->ret_stack, elem)

#define POP_RET_STACK   \
    Stack_Pop(&cpu->ret_stack)

#define GET_PUSH_ARG    \
    Get_Push_Arg(cpu, cpu->code[cpu->ip]);

#define GET_POP_ARG     \
    Get_Pop_Arg(cpu, cpu->code[cpu->ip]);

#define GET_JUMP_ARG    \
    Jump_Arg(cpu);

#define IP              \
    cpu->ip

#define CHECK_ZERO                              \
    if ((int) a == 0)                                 \
    {                                           \
        fprintf(stderr, "Division by zero!\n"); \
        mode = false;                           \
    }