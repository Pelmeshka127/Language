#include "dsl.h"

DEF_CMD(ADD, Op_Add, 
{
    return EVAL(LEFT) + EVAL(RIGHT);
})

DEF_CMD(SUB, Op_Sub, 
{
    return EVAL(LEFT) - EVAL(RIGHT);
})

DEF_CMD(MUL, Op_Mul,
{
    return EVAL(LEFT) * EVAL(RIGHT);
})

DEF_CMD(DIV, Op_Div,
{
    data_t a = EVAL(LEFT);
    data_t b = EVAL(RIGHT);
    if ((int) b == 0)
    {
        fprintf(stderr, "Division by zero in function %s\n", __PRETTY_FUNCTION__);
        fprintf(stderr, "The value of the tree is incorrect\n");
        return Incorrect_Node;
    }
    return a / b;
})

DEF_CMD(POW, Op_Pow,
{
    data_t a = EVAL(LEFT);
    data_t b = EVAL(RIGHT);
    data_t Muller = a;
    if ((int) a == 0)
        return 0;
    else if ((int) a == 1)
        return 1;
    else
    {
        for (int i = 1; i < (int) b; i++)
            a *= Muller;
        return a;
    }
})

DEF_CMD(SIN, Op_Sin,
{
    return (data_t) sin(EVAL(LEFT));
})

DEF_CMD(COS, Op_Cos,
{
    return (data_t) cos(EVAL(LEFT));
})

DEF_CMD(LN, Op_Ln,
{
    if (LEFT->data <= 0)
    {
        fprintf(stderr, "Incorrect argument in ln: %d\n", LEFT->data);
        fprintf(stderr, "The value of the tree is incorrect\n");
        return Incorrect_Node;
    }
    return (int) log(EVAL(LEFT));
})

DEF_CMD(EXP, Op_Exp, 
{
    return (data_t) exp(EVAL(LEFT));
})

DEF_CMD(SQRT, Op_Sqrt,
{
    return (data_t) sqrt(EVAL(LEFT));
})