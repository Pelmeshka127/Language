DEF_CMD(SIN, Op_Sin, 3,
{
    fprintf(dst_file, "sin\n");
})

DEF_CMD(COS, Op_Cos, 3,
{
    fprintf(dst_file, "cos\n");
})

DEF_CMD(LN, Op_Ln, 2,
{
    fprintf(dst_file, "ln\n");
})

DEF_CMD(EXP, Op_Exp, 3,
{
    fprintf(dst_file, "exp\n");
})

DEF_CMD(SMALL_DICK, Op_Below, 10, {})

DEF_CMD(NICE_SMALL_DICK, Op_Below_Eq, 15, {})

DEF_CMD(BIG_DICK, Op_Above, 8, {})

DEF_CMD(NICE_BIG_DICK, Op_Above_Eq, 13, {})

DEF_CMD(NICE_DICK, Op_Eql, 9, {})

DEF_CMD(ASS, Op_And, 3, {})

DEF_CMD(ORAL_SEX, Op_Or, 8, {})

DEF_CMD(PUSSY, Op_Not_Eql, 5, {})

DEF_CMD(IF, Op_If, 2, {})

DEF_CMD(ELIF, Op_Elif, 4, {})

DEF_CMD(ELSE, Op_Else, 4, {})

DEF_CMD(HORNY, Op_While, 5, {})

DEF_CMD(CUMMING, Op_End, 7, {
    fprintf(dst_file, "hlt\n");
})

DEF_CMD(START_SEX, Op_Start_Sex, 9, {})

DEF_CMD(END_SEX, Op_End_Sex, 7, {})

DEF_CMD(FISTING_ANAL, Op_Input, 12, {
    fprintf(dst_file, "in\n");
})

DEF_CMD(SUCK_DICK, Op_Print, 9, {
    fprintf(dst_file, "out\n");
})

DEF_CMD(+, Op_Add, 1,
{
    fprintf(dst_file, "add\n");
})

DEF_CMD(-, Op_Sub, 1,
{
    fprintf(dst_file, "sub\n");
})

DEF_CMD(*, Op_Mul, 1,
{
    fprintf(dst_file, "mul\n");
})

DEF_CMD(/, Op_Div, 1,
{
    fprintf(dst_file, "div\n");
})

DEF_CMD(^, Op_Pow, 1,
{
    fprintf(dst_file, "pow\n");
})

DEF_CMD(=, Op_Asg, 1, {

    int adress = Name_Table_Analyz(node, table);

    fprintf(dst_file, "pop [%d]\n", adress);
})

DEF_CMD(MILF, Op_Init, 4, {
    if (Name_Table_Analyz(node->left, table) == -1)
    {
        table->array[table->size].name = node->left->name;
        table->array[table->size].id   = table->size++;

        printf("%s\n", table->array[0].name);
        printf("%d\n", table->array[0].id);

        int adress = Name_Table_Analyz(node, table);

        fprintf(dst_file, "pop [%d]\n", adress);
    }

    else
    {
        fprintf(stderr, "AAAAA new initialized variable %s\n", node->name);
        return Incorrect_Var;
    }
})