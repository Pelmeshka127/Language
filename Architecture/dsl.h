#include <math.h>

#define NODE cur_node

#define LEFT cur_node->left

#define RIGHT cur_node->right

#define EVAL(node)  \
    Eval(node)

#define dL  \
    Diff_Calc(my_tree, LEFT)

#define dR  \
    Diff_Calc(my_tree, RIGHT)

#define cL  \
    Diff_Copy_Node(LEFT)

#define cR  \
    Diff_Copy_Node(RIGHT)

#define ADD(left, right, op_word) \
    Tree_New_Node(Op_Type, Op_Add, op_word, left, right)

#define SUB(left, right, op_word) \
    Tree_New_Node(Op_Type, Op_Sub, op_word, left, right)

#define MUL(left, right, op_word) \
    Tree_New_Node(Op_Type, Op_Mul, op_word, left, right)

#define DIV(left, right, op_word) \
    Tree_New_Node(Op_Type, Op_Div, op_word, left, right)

#define POW(left, right, op_word) \
    Tree_New_Node(Op_Type, Op_Pow, op_word, left, right)

#define SINUS(left, symbol)   \
    Tree_New_Node(Op_Type, Op_Sin, symbol, left, nullptr)

#define COSINUS(left, symbol)   \
    Tree_New_Node(Op_Type, Op_Cos, symbol, left, nullptr)

#define LOGARIFM(left, symbol)    \
    Tree_New_Node(Op_Type, Op_Ln, symbol, left, nullptr)

#define EXPONENTA(left, symbol)   \
    Tree_New_Node(Op_Type, Op_Exp, symbol, left, nullptr)

#define SQUARE(left, symbol)  \
    Tree_New_Node(Op_Type, Op_Sqrt, symbol, left, nullptr)

#define New_Num(data) \
    Tree_New_Node(Num_Type, data)

#define New_Var(var_name, var_type)    \
    Tree_New_Node(Var_Type, var_type, var_name)

#define ASSIGNMENT(left, right, symbol)  \
    Tree_New_Node(Op_Type, Op_Asg, symbol, left, right)

#define New_Connect_Type(left, right)   \
    Tree_New_Node(Connect_Type, 0, nullptr, left, right)

#define WHILE_NODE(condition, body, op_word)  \
    Tree_New_Node(Op_Type, Op_While, op_word, condition, body)

#define TOKEN_TYPE(token_type)    \
    tokens->array[tokens->size].type == token_type

#define TOKEN_DATA  \
    tokens->array[tokens->size].data

#define FUNC_DECLARATION "step brother"

#define MAIN_FUNCTION "hentai"

#define IF_WORD "dildo"

#define ELIF_WORD "anal_beads"

#define ELSE_WORD "strapon"