DEF_CMD(+, Op_Add, 1, "+",
{
    Parse_Node(dst_file, node->left, table);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "add\n");
})

DEF_CMD(-, Op_Sub, 1, "-",
{
    Parse_Node(dst_file, node->left, table);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "sub\n");
})

DEF_CMD(*, Op_Mul, 1, "*",
{
    Parse_Node(dst_file, node->left, table);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "mul\n");
})

DEF_CMD(/, Op_Div, 1, "/",
{
    Parse_Node(dst_file, node->left, table);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "div\n");
})

DEF_CMD(^, Op_Pow, 1, "^",
{
    Parse_Node(dst_file, node->left, table);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "pow\n");
})

DEF_CMD(=, Op_Asg, 1, "=",
{
    int adress = Name_Table_Analyz(node->left, table);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "pop [%d]\n", adress);
})

DEF_CMD(SIN, Op_Sin, 3, "sin",
{
    fprintf(dst_file, "sin\n");
})

DEF_CMD(COS, Op_Cos, 3, "cos",
{
    fprintf(dst_file, "cos\n");
})

DEF_CMD(LN, Op_Ln, 2, "ln",
{
    fprintf(dst_file, "ln\n");
})

DEF_CMD(EXP, Op_Exp, 3, "exp",
{
    fprintf(dst_file, "exp\n");
})

DEF_CMD(SQRT, Op_Sqrt, 4, "sqrt",
{
    Parse_Node(dst_file, node->left, table);
    fprintf(dst_file, "sqrt\n");
})


DEF_CMD(SMALL, Small, 5, "", {})

DEF_CMD(BIG, Big, 3, "", {})

DEF_CMD(NICE, Nice, 4, "", {})

DEF_CMD(DICK, Dick, 4, "", {})

DEF_CMD(SUCK, Suck, 4, "", {})

DEF_CMD(ANAL, Anal, 4, "", {})

DEF_CMD(BEADS, Beads, 5, "", {})

DEF_CMD(FISTING, Fisting, 7, "", {})

DEF_CMD(START, Start, 5, "", {})

DEF_CMD(END, End, 3, "", {})

DEF_CMD(SEX, Sex, 3, "", {})

DEF_CMD(STEP, Step, 4, "", {})

DEF_CMD(BROTHER, Brother, 7, "", {})



DEF_CMD(HENTAI, Op_Main, 6, MAIN_FUNCTION, {})

DEF_CMD(IM, Op_Im, 2, "im", {})

DEF_CMD(STUCK, Op_Stuck, 5, "stuck", {})



DEF_CMD(SMALL_DICK, Op_Below, 10, "<", {})

DEF_CMD(NICE_SMALL_DICK, Op_Below_Eq, 15, "<=", {})

DEF_CMD(BIG_DICK, Op_Above, 8, ">", {})

DEF_CMD(NICE_BIG_DICK, Op_Above_Eq, 13, ">=", {})

DEF_CMD(NICE_DICK, Op_Eql, 9, "==", {})

DEF_CMD(PUSSY, Op_Not_Eql, 5, "!=", {})

DEF_CMD(DILDO, Op_If, 5, "if", {
    Parse_Label(dst_file, node->left->left, table, If_Count, node->name);
    fprintf(dst_file, "jump :if_not_%d\n", If_Count);
    fprintf(dst_file, "if_%d:\n", If_Count++);
    Parse_Node(dst_file, node->left->right, table);
    fprintf(dst_file, "jump :end_if_%d\n", If_Count - 1);
    fprintf(dst_file, "if_not_%d:\n", If_Count - 1);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "end_if_%d:\n", If_Count - 1);
})

DEF_CMD(ANAL_BEADS, Op_Elif, 10, "elif", {
    Parse_Label(dst_file, node->left->left, table, Elif_Count, node->name);
    fprintf(dst_file, "jump :elif_not_%d\n", Elif_Count);
    fprintf(dst_file, "elif_%d:\n", Elif_Count++);
    Parse_Node(dst_file, node->left->right, table);
    fprintf(dst_file, "jump :end_elif_%d\n", Elif_Count - 1);
    fprintf(dst_file, "elif_not_%d:\n", Elif_Count - 1);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "end_elif_%d:\n", Elif_Count - 1);
})

DEF_CMD(STRAPON, Op_Else, 7, "else", {
    fprintf(dst_file, "else_%d:\n", Else_Count++);
    Parse_Node(dst_file, node->left, table);
})

DEF_CMD(HORNY, Op_While, 5, "while", {
    fprintf(dst_file, "while_%d:\n", While_Count);
    Parse_Label(dst_file, node->left, table, While_Count, node->name);
    Parse_Node(dst_file, node->right, table);
    fprintf(dst_file, "jump while_%d\n", While_Count);
    fprintf(dst_file, "end_while_%d:\n", While_Count++);
})

DEF_CMD(CUMMING, Op_End, 7, "exit", {
    fprintf(dst_file, "ret\n");
})

DEF_CMD(START_SEX, Op_Start_Sex, 9, "{", {})

DEF_CMD(END_SEX, Op_End_Sex, 7, "}", {})

DEF_CMD(FISTING_ANAL, Op_Input, 12, "input", {
    fprintf(dst_file, "in\n");
    fprintf(dst_file, "pop [%d]\n", Name_Table_Analyz(node->left, table));
})

DEF_CMD(SUCK_DICK, Op_Print, 9, "print", {
    //fprintf(dst_file, "push [%d]\n", Name_Table_Analyz(node->left, table));
    Parse_Node(dst_file, node->left, table);
    fprintf(dst_file, "out\n");
})

DEF_CMD(MILF, Op_Init, 4, "init", {
    if (Name_Table_Analyz(node->left, table) == -1)
    {
        strncpy(table->array[table->size].name, node->left->name, strlen(node->left->name));
        table->array[table->size].id   = table->size;

        Parse_Node(dst_file, node->right, table);
        fprintf(dst_file, "pop [%d]\n", table->size++);
    }

    else
    {
        fprintf(stderr, "AAAAA new initialized variable %s\n", node->name);
        return Incorrect_Var;
    }
})

DEF_CMD(BOOBS, Op_Ret, 5, "return", {
    Parse_Node(dst_file, node->left, table);
    fprintf(dst_file, "ret\n");
})