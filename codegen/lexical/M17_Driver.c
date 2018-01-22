#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "M17_StateSwitch.h"
//#include "Input.h"
#include "../parser/M17_Parser.h"


int main( int argc, char * argv[] )
{
    if( argc < 2 )
    {
        printf( "USAGE: ./m17 <file>\n" );
        exit( 0 );
    }

    char * filename = argv[1];

    Token * token = malloc( sizeof( *token ) );

    if( setupAnalyzer( filename, token ) == 0 )
    {
        DEBUG;
        exit( 0 );
    }

    parserInit();
    // printf( "I'm FInished!!\n" );

    //while loop function needs to be passed to advance() in M17_Parser.c
    // while( ( getToken( token ) ) != 0 )
    // {
    //     if( strncmp( token->key, "-", 1 ) != 0 )
    //     {
    //         printf( "Line %d : %d : %s\n", token->lineNumber, token->id, token->key );
    //         memset( token->key, '-', 2048 );
    //         token->id = 0;
    //     }
    // }
    destroyAnalyzer();

    return 0;
}
