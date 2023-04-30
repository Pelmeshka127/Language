#ifndef TEXT_FUNCTIONS_H_
#define TEXT_FUNCTIONS_H_

#include "../Architecture/architecture.h"

struct Text_Info {
    char * buffer;
    int symbols_count;
    int lines_count;
    char ** pointers;
    int char_num;
};

#define ONEGIN_ERROR() \
fprintf(stderr, "There is an error in %s, in %d: %s\n", \
        __PRETTY_FUNCTION__, __LINE__, strerror(errno))

//---------------------------------------------------------------------------------------------//

/// function reads input file
int Onegin_Read(Text_Info * const Onegin, FILE * fp);

//---------------------------------------------------------------------------------------------//

/// function writes strings to the out file 
void Onegin_Print_To_File(Text_Info * const Onegin, FILE * fp);

//---------------------------------------------------------------------------------------------//

/// function closes files and clear dedicated memory
void Onegin_Dtor(Text_Info * const Onegin);

//---------------------------------------------------------------------------------------------//

#endif