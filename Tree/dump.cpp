#include "dump.h"

//-------------------------------------------------------------------------------//

static FILE *log_file = nullptr;

static FILE *graph_file = nullptr;

static int graph_num  = 1;

//-------------------------------------------------------------------------------//

int Open_Log_File()
{
    log_file = fopen("graphics/log.html", "w");
    if (log_file == nullptr)
    {
        fprintf(stderr, "Failed openning log.html in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }
    return No_Error;
}

//-------------------------------------------------------------------------------//

int Close_Log_File()
{
    if (fclose(log_file) == EOF)
    {
        fprintf(stderr, "Failed closing log.html in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }
    return No_Error;
}

//-------------------------------------------------------------------------------//

int Tree_Dump(tree_s * const my_tree)
{
#ifdef TREE_DUMP

    assert(my_tree);

    graph_file = fopen("graphics/graph.dot", "w");
    if (graph_file == nullptr)
    {
        fprintf(stderr, "Failed openning graph.dot in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    Tree_Get_Size(my_tree, my_tree->root);

    fprintf(graph_file, "digraph Tree\n{\n");
    fprintf(graph_file, "   rankdir = HR;\n");
    fprintf(graph_file, "   node[fontsize=14];\n   edge[color=\"black\",fontcolor=\"blue\",fontsize=12];\n");
    fprintf(graph_file, "   tree[shape = Mrecord, style = filled, fillcolor = \"red\", "
                        "label = \"My Tree | size = %d\"];\n", my_tree->size);
    Tree_Draw(my_tree->root);
    fprintf(graph_file, "   tree -> \"%p\" [color = \"red\"];\n", my_tree->root);
    fprintf(graph_file, "}");

    if (fclose(graph_file) == EOF)
    {
        fprintf(stderr, "Failed closing graph.dot in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    char call_graph[100] = " ";

    sprintf(call_graph, "dot graphics/graph.dot -Tpng -o graphics/graph%d.png", graph_num++);
    
    system(call_graph);

    fprintf(log_file, "<pre>\n");
    fprintf(log_file, "\n<image src = \"gpaph%d.png\" /image>\n\n\n", graph_num - 1);

    return No_Error;

#endif
}

//-------------------------------------------------------------------------------//

void Tree_Draw(tree_node * const root)
{
    assert(root);

    if (root->type == Op_Type)
    {
        char oper_symbol[Oper_Len] = "";
        Tree_Get_Operator_By_Number(root->data, oper_symbol, Oper_Len);
        fprintf(graph_file, "   \"%p\"[shape = Mrecord, style = filled, fillcolor = \"cornflowerblue\","
                            "   label = \" <value> %s\"];\n", root, root->name);
    }

    else if (root->type == Num_Type)
        fprintf(graph_file, "   \"%p\"[shape = Mrecord, style = filled, fillcolor = \"pink1\","
                            "   label = \" <value> %d | NUM  \"];\n", root, root->data);
    else if (root->type == Var_Type)
        fprintf(graph_file, "   \"%p\"[shape = Mrecord, style = filled, fillcolor = \"yellow\","
                            "   label = \" <value> %s | var_data =  %d\"];\n", root, root->name, root->data);

    else if (root->type == Connect_Type)
        fprintf(graph_file, "   \"%p\"[shape = Mrecord, style = filled, fillcolor = \"blanchedalmond\","
                            "   label = \" <value> Connect_Node\"];\n", root);
    
    if (root->left != nullptr)
        fprintf(graph_file, "  \"%p\" -> \"%p\" [color = \"green\"];\n", root, root->left);

    if (root->right != nullptr)
        fprintf(graph_file, "  \"%p\" -> \"%p\" [color = \"red\"];\n", root, root->right);

    if (root->left != nullptr)
        Tree_Draw(root->left);

    if (root->right != nullptr)
        Tree_Draw(root->right);
}

//-------------------------------------------------------------------------------//

void Tree_Dump_To_Tex(tree_s * const my_tree, char * png_name)
{
    Tree_Dump(my_tree);

    sprintf(png_name, "graphics/graph%d.png", graph_num - 1);
}


int Tree_Dump_Node(tree_node * const node)
{
    graph_file = fopen("graphics/graph.dot", "w");
    if (graph_file == nullptr)
    {
        fprintf(stderr, "Failed openning graph.dot in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    fprintf(graph_file, "digraph Tree\n{\n");
    fprintf(graph_file, "   rankdir = HR;\n");
    fprintf(graph_file, "   node[fontsize=14];\n   edge[color=\"black\",fontcolor=\"blue\",fontsize=12];\n");

    Tree_Draw(node);
    fprintf(graph_file, "}");

    if (fclose(graph_file) == EOF)
    {
        fprintf(stderr, "Failed closing graph.dot in function %s\n", __PRETTY_FUNCTION__);
        return File_Error;
    }

    char call_graph[100] = " ";

    sprintf(call_graph, "dot graphics/graph.dot -Tpng -o graphics/graph_node%d.png", graph_num++);
    
    system(call_graph);

    fprintf(log_file, "<pre>\n");
    fprintf(log_file, "\n<image src = \"gpaph_node%d.png\" /image>\n\n\n", graph_num - 1);

    return No_Error;
}