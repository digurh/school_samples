#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//#include "Input.h"
#include "StringMaker.h"
//#include "M17_StateFunctions.h"
#include "M17_StateSwitch.h"


static Input * input;

int setupAnalyzer( char * filename, Token * token )
{
    int fd = open( filename, 'r' );

    input = malloc( sizeof( *input ) );

    if( input == NULL )
    {
        DEBUG;
        return 0;
    }
    setupInput( input, fd );

    token->id = 0;
    token->lineNumber = 0;

    return 1;
}

void destroyAnalyzer()
{
    closeInput( input );
}

int getToken( Token * token )
{
    int state = 0;
    char * error = "Error, invalid token.";

    StringMaker * sm = malloc( 2048 );

    do {
        switch( state )
        {
            case 0:
                state = zeroState( input, sm );
                break;
            case 1:
                state = forwardSlash( input );
                break;
            case 2:
                state = blockComment( input );
                break;
            case 3:
                state = makeWord( input, sm );
                break;
            case 4:
                state = stringLiteral( input, sm );
                break;
            case 5:
                state = decimalConstant( input, sm );
                break;
            case 6:
                state = zeroConstant( input, sm );
                break;
            case 7:
                state = slAbsorb( token, sm );
                break;
            case 8:
                state = mwAbsorb( token, sm );
                break;
            case 9:
                state = comment( input );
                break;
            case 10:
                state = asterisk( input );
                break;
            case 11:
                state = escapeChar( input, sm );
                break;
            case 12:
                state = decimalPoint( input, sm );
                break;
            case 13:
                state = decAbsorb( token, sm );
                break;
            case 14:
                state = hexConstant( input, sm );
                break;
            case 15:
                state = octalConstant( input, sm );
                break;
            case 16:
                state = floatConstant( input, sm );
                break;
            case 17:
                state = hexAbsorb( token, sm );
                break;
            case 18:
                state = octAbsorb( token, sm );
                break;
            case 19:
                state = floatAbsorb( token, sm );
                break;
            case 20:
                state = sciEe( input, sm );
                break;
            case 21:
                state = errorAbsorb( token );
                break;
            case 22:
                state = metaChar( input, sm );
                break;
            case 23:
                state = pointerMul( token, sm );
                break;
            case 24:
                state = plus( token, sm );
                break;
            case 25:
                state = minus( token, sm );
                break;
            case 26:
                state = parenthesis( token, sm );
                break;
            case 27:
                state = curlyBrace( token, sm );
                break;
            case 28:
                state = bracket( token, sm );
                break;
            case 29:
                state = greaterThan( input, sm );
                break;
            case 30:
                state = lessThan( input, sm );
                break;
            case 31:
                state = equals( input, sm );
                break;
            case 32:
                state = gtAbsorb( token, sm );
                break;
            case 33:
                state = ltAbsorb( token, sm );
                break;
            case 34:
                state = eqAbsorb( token, sm );
                break;
            case 35:
                state = comma( token, sm );
                break;
            case 36:
                state = semicolon( token, sm );
                break;
            case 37:
                state = logNot( input, sm );
                break;
            case 38:
                state = modulus( token, sm );
                break;
            case 39:
                state = dotOp( token, sm );
                break;
            case 40:
                state = andOp( input, sm );
                break;
            case 41:
                state = tildeOp( token, sm );
                break;
            case 42:
                state = notAbsorb( token, sm );
                break;
            case 43:
                state = orOp( input, sm );
                break;
            case 44:
                state = orAbsorb( token, sm );
                break;
            case 45:
                state = andAbsorb( token, sm );
                break;
            case 46:
                state = divAbsorb( token, sm );
                break;
            default:
                strcpy( token->key, error );
                return -1;
        }
    } while( state >= 0 );
    if( state == -1 ) return state;
    return 0;
}
