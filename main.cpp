#include "Tree/tree.h"
#include "Tree/dump.h"
#include "Parsering/parser.h"

int main(int argc, char ** argv)
{
    if (Check_Cmdline_Arg(argc) == Cmdline_Error)
        return Cmdline_Error;

    Open_Log_File();

    tree_s my_tree = {};

    Tree_Ctor(&my_tree);

    // Tree_Download(&my_tree, argv[1]);

    Front_End(&my_tree, argv[1]);

    printf("FRONT_END done\n");

    Make_Ast_Tree(&my_tree);

    Tree_Dtor(&my_tree);

    Tree_Ctor(&my_tree);

    Read_Ast_Tree(&my_tree);

    Back_End(&my_tree);

    Tree_Dtor(&my_tree);

    Close_Log_File();

    return 0;
}