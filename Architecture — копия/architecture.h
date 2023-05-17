#ifndef ARCHITECTURE_H_
#define ARCHITECTURE_H_

struct cmd_info {
    int CP;
    size_t file_size;
};

enum CMD {
    CMD_PUSH     = 1,
    CMD_POP      = 2,
    CMD_ADD      = 3,
    CMD_SUB      = 4,
    CMD_MUL      = 5,
    CMD_DIV      = 6,
    CMD_SQRT     = 7,
    CMD_MINUS    = 8,
    CMD_IN       = 9,
    CMD_OUT      = 10,

    // TODO remove - <done>

    CMD_INF      = 13,
    CMD_ZERO     = 14,
    CMD_POW      = 15,

    CMD_RAM      = 19,
    CMD_CIRCLE   = 20,

    CMD_JUMP     = 21,
    CMD_JB       = 22,
    CMD_JBE      = 23,
    CMD_JA       = 24,
    CMD_JAE      = 25,
    CMD_JE       = 26,
    CMD_JNE      = 27,

    CMD_RET      = 30,
    CMD_CALL     = 31,

    CMD_HLT      = 0,
};

enum Erros {
    No_Error             = 1 << 10,
    Cmd_Line_Arg_Err     = 1 << 11,
    Src_File_Err         = 1 << 12,
    Reading_File_Err     = 1 << 13,
    Alloc_Err            = 1 << 14,
    Undefined_Cmd        = 1 << 15,
    Undefined_Arg        = 1 << 16,
    Extra_Arg            = 1 << 17,
    Incorrext_Cmd_Arg    = 1 << 18,
    Asm_Compile_Error    = 1 << 19,
    CP_Error             = 1 << 20,
    CPU_Compile_Error    = 1 << 21,
    Label_Error          = 1 << 22,
    Writing_Error        = 1 << 23,
    Disasm_Compile_Error = 1 << 24,
};

enum Args {
    CMD_MASK   = 0x1F,
    ARG_IMMED  = 1 << 5,
    ARG_REG    = 1 << 6,
    ARG_RAM    = 1 << 7,
    Digit      = 1,
    Reg        = 2,
    Label      = 3,
};

#endif