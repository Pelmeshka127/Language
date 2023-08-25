DEF_CMD(SMALL_DICK, Op_Below, 10, "<", {})

DEF_CMD(NICE_SMALL_DICK, Op_Below_Eq, 15, "<=", {})

DEF_CMD(BIG_DICK, Op_Above, 8, ">", {})

DEF_CMD(NICE_BIG_DICK, Op_Above_Eq, 13, ">=", {})

DEF_CMD(NICE_DICK, Op_Eql, 9, "==", {})

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

DEF_CMD(STEP_BROTHER, Op_Step_Brother, 12, FUNC_DECLARATION, {})