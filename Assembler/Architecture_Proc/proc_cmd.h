#include "proc_dsl.h"

DEF_CMD(HLT, CMD_HLT, 0, 
{
    printf("The end\n");
    mode = false;
})

DEF_CMD(PUSH, CMD_PUSH, Digit, 
{
    int elem = GET_PUSH_ARG;
    PUSH_STACK(elem);
})


DEF_CMD(POP, CMD_POP, Digit, 
{
    int * value = GET_POP_ARG;
    * value = (int) POP_STACK;
})


DEF_CMD(ADD, CMD_ADD, 0, 
{
    PUSH_STACK(POP_STACK + POP_STACK);
})

DEF_CMD(SUB, CMD_SUB, 0, 
{
    PUSH_STACK(POP_STACK - POP_STACK);
})

DEF_CMD(MUL, CMD_MUL, 0,
{
    PUSH_STACK(POP_STACK * POP_STACK);
})

DEF_CMD(DIV, CMD_DIV, 0,
{
    double a = POP_STACK; // elem_t 
    double b = POP_STACK; // TODO div 0 -<done>
    CHECK_ZERO;
    double c = b / a;
    PUSH_STACK(c);
})

DEF_CMD(SQRT, CMD_SQRT, 0,
{
    PUSH_STACK(sqrt(POP_STACK));
})

DEF_CMD(MINUS, CMD_MINUS, 0,
{
    PUSH_STACK(-1 * POP_STACK);
})

DEF_CMD(IN, CMD_IN, 0,
{
    int value = 0;
    scanf("%d", &value);
    PUSH_STACK(value);
})

DEF_CMD(POW, CMD_POW, 0,
{
    elem_t value = POP_STACK;
    PUSH_STACK(value * value);
})

DEF_CMD(OUT, CMD_OUT, 0,
{
    elem_t value = POP_STACK;
    printf("The result is %.2f\n", value);
})

DEF_CMD(CALL, CMD_CALL, Label,
{
    IP++;
    PUSH_RET_STACK(IP + sizeof(int) - 1); // int -> addr_t; rearrange calls
    IP = GET_JUMP_ARG;            // TODO remove cpu -> add dsl - <done>
})

DEF_CMD(RET, CMD_RET, 0,
{
    IP = (size_t) POP_RET_STACK;
})

DEF_CMD(JUMP, CMD_JUMP, Label, 
{
    IP++;
    IP = GET_JUMP_ARG;
})  

DEF_CMD(JB, CMD_JB, Label, 
{
    IP++;
    // elem_t addr = (addr_t) GET_VALUE_FROM_BINARY
    if (POP_STACK < POP_STACK)
    {
        IP = GET_JUMP_ARG;
    }
    else
        IP += sizeof(int) - 1;
})

DEF_CMD(JBE, CMD_JBE, Label, 
{
    IP++;
    if (POP_STACK <= POP_STACK)
    {
        IP = GET_JUMP_ARG;
    }
    else 
        IP += sizeof(int) - 1;
})

DEF_CMD(JA, CMD_JA, Label, 
{
    IP++;
    if (POP_STACK > POP_STACK)
    {
        IP = GET_JUMP_ARG;
    }
    else 
        IP += sizeof(int) - 1;
})

DEF_CMD(JAE, CMD_JAE, Label, 
{
    IP++;
    if (POP_STACK >= POP_STACK)
    {
        IP = GET_JUMP_ARG;
    }
    else 
        IP += sizeof(int) - 1;
})

DEF_CMD(JE, CMD_JE, Label,
{
    IP++;
    if ((int) POP_STACK == (int) POP_STACK)
    {
        IP = GET_JUMP_ARG;
    }
    else 
        IP += sizeof(int) - 1;
})

DEF_CMD(JNE, CMD_JNE, Label,
{
    IP++;
    if ((int) POP_STACK != (int) POP_STACK)
    {
        IP = GET_JUMP_ARG;
    }
    else 
        IP += sizeof(int) - 1;
})

DEF_CMD(RAM, CMD_RAM, 0,
{
    for (int i = 0; i < Def_Ram_Size; i++)
    {
        if (i % 60 == 0)
            printf("\n");
        if (cpu->RAM[i] == 1)
            printf("o ");
        if (cpu->RAM[i] == 0)
            printf("- ");
    }
    printf("\n");
})

DEF_CMD(INF, CMD_INF, 0,
{
    printf("Inf roots\n");
})

DEF_CMD(ZERO, CMD_ZERO, 0,
{
    printf("No roots\n");
})

DEF_CMD(CIRCLE, CMD_CIRCLE, 0,
{
    FILE * fp = nullptr;
    fp = fopen("..//Tests/circle_show.txt", "w");
    for (int i = 0; i < Def_Ram_Size; i++)
    {
        if (i % 60 == 0)
            fprintf(fp, "\n");
        if (cpu->RAM[i] == 1)
            fprintf(fp, "0 ");
        if (cpu->RAM[i] == 0)
            fprintf(fp, "- ");
    }
    fprintf(fp, "\n");
    fclose(fp);
})