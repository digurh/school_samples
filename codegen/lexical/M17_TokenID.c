#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "M17_TokenID.h"


ID getWordID( char * word )
{
    ID i;
    //printf( "%s", word );
    char * symbols[] = {"char",
                        "int",
                        "float",
                        "struct",
                        "void",
                        "while",
                        "do",
                        "for",
                        "if",
                        "else",
                        "return",
                        "read",
                        "write",
                        "allocate",
                        "deallocate",
                        "func"};

    for( i = CHAR_ID; i < IDENTIFIER_ID; i++ )
    {
        if( strncmp( word, symbols[i], sizeof( symbols[i] ) - 1 ) == 0 ) return i;
    }
    return i;
}
