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

#define ADD(left, right) \
    Tree_New_Node(Op_Type, Op_Add, nullptr, left, right)

#define SUB(left, right) \
    Tree_New_Node(Op_Type, Op_Sub, nullptr, left, right)

#define MUL(left, right) \
    Tree_New_Node(Op_Type, Op_Mul, nullptr, left, right)

#define DIV(left, right) \
    Tree_New_Node(Op_Type, Op_Div, nullptr, left, right)

#define POW(left, right) \
    Tree_New_Node(Op_Type, Op_Pow, nullptr, left, right)

#define SIN(left)   \
    Tree_New_Node(Op_Type, Op_Sin, nullptr, left, nullptr)

#define COS(left)   \
    Tree_New_Node(Op_Type, Op_Cos, nullptr, left, nullptr)

#define LN(left)    \
    Tree_New_Node(Op_Type, Op_Ln, nullptr, left, nullptr)

#define EXP(left)   \
    Tree_New_Node(Op_Type, Op_Exp, nullptr, left, nullptr)

#define New_Num(data) \
    Tree_New_Node(Num_Type, data)

#define New_Var(data)  \
    Tree_New_Node(Var_Type, data)

#define New_Var_New(var)    \
    Tree_New_Node(Var_Type, 0, var)

#define EQUAL(left, right)  \
    Tree_New_Node(Op_Type, Op_Eql, nullptr, left, right)

#define New_Connect_Type(left, right)   \
    Tree_New_Node(Connect_Type, 0, nullptr, left, right)

#define IF(condition, body) \
    Tree_New_Node(Op_Type, Op_If, "if", condition, body)