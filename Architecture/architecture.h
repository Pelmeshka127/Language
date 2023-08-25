#ifndef ARCHITECTURE_H_
#define ARCHITECTURE_H_

enum ErrorsCode {
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

enum TokenTypes {
    No_Type             = 0,
    Num_Type            = 1,
    Op_Type             = 2,
    Var_Type            = 3,
    Connect_Type        = 4,
    Function_Type       = 5,
    Double_Word_Type    = 6,
};

enum VarTypes {
    Default_Var = 1,
    Param_Var   = 2,
};

enum FunctionTypes {
    Op_Dec_Func     = 1,
    Op_Func_Name    = 2,
};

enum OperationTypes {
    Op_Asg          = 1,
    Op_Add          = 2,
    Op_Sub          = 3,
    Op_Mul          = 4,
    Op_Div          = 5,
    Op_Pow          = 6,
    Op_If           = 7,
    Op_Else         = 8,
    Op_While        = 9,
    Op_Not_Eql      = 10,
    Op_Ret          = 11,
    Op_Init         = 12,
    Op_Sin          = 13,
    Op_Cos          = 14,
    Op_Ln           = 15,
    Op_Exp          = 16,
    Op_Left_Br      = 17,
    Op_Right_Br     = 18,
    Op_End          = 19,
    Op_Sqrt         = 20,
    Op_Comma        = 21,
    Op_Main         = 22,
    Op_Im           = 23,
    Op_Stuck        = 24,
};

enum SimpleWordOperationTypes {
    Small   = 40,
    Big     = 41,
    Nice    = 42,
    Suck    = 43,
    Dick    = 44,
    Anal    = 45,
    Beads   = 46,
    Start   = 47,
    End     = 48,
    Sex     = 49,
    Fisting = 50,
    Step    = 51,
    Brother = 52,
};

enum DoubleWordsOperationTypes {
    Op_Below        = Small   + Dick,
    Op_Above        = Big     + Dick,
    Op_Eql          = Nice    + Dick,
    Op_Print        = Suck    + Dick,
    Op_Elif         = Anal    + Beads,
    Op_Input        = Fisting + Anal,
    Op_Start_Sex    = Start   + Sex,
    Op_End_Sex      = End     + Sex,
    Op_Step_Brother = Step    + Brother,
    Op_Below_Eq     = Nice    + Small    + Dick, 
    Op_Above_Eq     = Nice    + Big      + Dick,
};

const int Oper_Len   = 15;

const int Max_Length = 40;

#endif