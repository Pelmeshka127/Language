#include "translator.h"



int Translator_Reader(tree_s *src_tree)
{
    FILE *translate_file = fopen("translate_file.txt", "w");
    if (translate_file == nullptr)
    {
        fprintf(stderr, "Error in openning translate file in %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    Translator_Print(src_tree->root, translate_file);

    if (fclose(translate_file) == EOF)
    {
        fprintf(stderr, "Error in closin translate file in %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    return No_Error;
}



int Translator_Print(tree_node * const cur_node, FILE *dst_file)
{
    if (cur_node == nullptr)
        return No_Error;

    fprintf(dst_file, " ( ");

    Translator_Print(cur_node->left, dst_file);

    Translator_Print(cur_node->right, dst_file);

    if (cur_node->type == Num_Type)
        fprintf(dst_file, "%d", cur_node->data);

    else if (cur_node->type == Op_Type)
        fprintf(dst_file, "%s", cur_node->name);

    else if (cur_node->type == Var_Type)
        fprintf(dst_file, "%s", cur_node->name);

    fprintf(dst_file, " ) ");

    return No_Error;
}

// int Tree_Print_In_Order(tree_node * const cur_node, FILE * dst_file)
// {
//     if (cur_node == nullptr)
//         return No_Error;

//     fprintf(dst_file, " ( ");    

//     Tree_Print_In_Order(cur_node->left, dst_file);    

//     if (cur_node->type == Num_Type)
//         fprintf(dst_file, "%d", cur_node->data);
        
//     else if (cur_node->type == Op_Type)
//     {   
//         char oper_symbol[Oper_Len] = "";
//         Tree_Get_Operator_By_Number(cur_node->data, oper_symbol, Oper_Len);
//         fprintf(dst_file, "%s", oper_symbol);
//     }

//     else if (cur_node->type == Var_Type)
//         fprintf(dst_file, "%c", Tree_Get_Variable_By_Number(cur_node->data));

//     Tree_Print_In_Order(cur_node->right, dst_file);

//     fprintf(dst_file, " ) ");
    
//     return No_Error;
// }f

int Make_Asm_Code(FILE *src_file)
{
    Text_Info onegin = {};
    
    if (Onegin_Read(&onegin, src_file) != No_Error)
    {
        fprintf(stderr, "Failed parsering in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    onegin.char_num = 0;

    Onegin_Dtor(&onegin);

    return No_Error;   
}