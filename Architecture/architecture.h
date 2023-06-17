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

enum Types {
    Num_Type      = 1,
    Op_Type       = 2,
    Var_Type      = 3,
    Connect_Type  = 4,
    Function_Type = 5,
};

enum Var_Types {
    Default_Var = 1,
    Param_Var   = 2,
};

enum Function_Types {
    Op_Dec_Func     = 1,
    Op_Func_Name    = 2,
};

enum Operation_Types {
    Op_Asg          = 1,
    Op_Add          = 2,
    Op_Sub          = 3,
    Op_Mul          = 4,
    Op_Div          = 5,
    Op_Pow          = 6,
    Op_If           = 7,
    Op_Elif         = 8,
    Op_Else         = 9,
    Op_While        = 10,
    Op_Eql          = 12,
    Op_Not_Eql      = 13,
    Op_Below        = 14,
    Op_Below_Eq     = 15,
    Op_Above        = 16,
    Op_Above_Eq     = 17,
    Op_Ret          = 18,
    Op_Init         = 19,
    Op_Sin          = 20,
    Op_Cos          = 21,
    Op_Ln           = 22,
    Op_Exp          = 23,
    Op_Start_Sex    = 24,
    Op_End_Sex      = 25,
    Op_Left_Br      = 26,
    Op_Right_Br     = 27,
    Op_Input        = 28,
    Op_Print        = 29,
    Op_End          = 30,
    Op_Sqrt         = 31,
};



const int Oper_Len   = 15;

const int Max_Length = 40;

#endif