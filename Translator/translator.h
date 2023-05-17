#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include "../Tree/tree.h"
#include "../Front_End/frontend.h"

int Translator_Reader(tree_s *src_tree);

int Translator_Print(tree_node * const cur_node, FILE *dst_file);

int Make_Asm_Code(FILE *src_file);

#endif