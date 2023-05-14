#ifndef ARCHITECTURE_H_
#define ARCHITECTURE_H_

enum Errors_Code {
    No_Error        = 0,
    Bad_Alloc       = 1,
    File_Error      = 2,
    Undef_Capacity  = 3,
    Incorrect_Node  = 4,
    Underflow       = 5,
    Overflow        = 6,
    Incorrect_Mode  = 7,
    Cmdline_Error   = 8,
    Empty_Node      = 9,
    Incorrect_Type  = 20,
    Incorrect_Var   = 21,
};

const int Num_Type      = 1;

const int Op_Type       = 2;

const int Var_Type      = 3;

const int Connect_Type  = 4;

enum Operation_Types {
    Op_Add      = 1,
    Op_Sub      = 2,
    Op_Mul      = 3,
    Op_Div      = 4,
    Op_Pow      = 5,
    Op_Sin      = 6,
    Op_Cos      = 7,
    Op_Ln       = 8,
    Op_Exp      = 9,
    Op_Asg      = 10,
    Op_If       = 11,
    Op_And      = 12,
    Op_Or       = 13,
    Op_Below     = 14,
    Op_Below_Eq  = 15,
    Op_Above     = 16,
    Op_Above_Eq  = 17,
    Op_Eql    = 18,
    Op_Elif     = 19,
    Op_Else     = 20,
    Op_Not_Eql= 21,
    Op_While    = 22,
    Op_Start_Sex = 23,
    Op_End_Sex   = 24,
    Op_End      = 30,
};

enum Var_Types {
    Var_X   = 1,
    Var_Y   = 2,
    Var_Z   = 3,
};

enum Programm_Mode {
    Show_Tree                   = 1,
    Find_Func_Value_At_Point    = 2,
    Get_Diff                    = 3,
    Find_Diff_Value_At_Point    = 4,
    Show_Diff_Tree              = 5,
    Maclaurin                   = 6,
    Finish_Prog                 = 10,
};

const int Pre_Order  = 1;

const int In_Order   = 2;

const int Post_Order = 3;

const int Oper_Len   = 15;

const int Max_Length = 40;

#endif