#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Input.h"
//#include "StringMaker.h"
#include "M17_StateFunctions.h"
#include "M17_TokenID.h"


static unsigned int lineNumber = 1;;

int advance( Input * input )
{
    int c;

    if( ( c = readChar( input ) ) == '\n' ) lineNumber++;

    return c;
}

int zeroState( Input * input, StringMaker * sm )
{
    int c;

    if( ( c = advance( input ) ) == -1 ) return -2;

    //if( isspace( c ) ) return 0;
    if( !isgraph( c ) ) return 0;
    else if( c == '/' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 1;
    }
    else if( isalpha( c ) )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 3;
    }
    else if( c == '\"' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 4;
    }
    else if( isdigit( c ) && c != '0' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 5;
    }
    else if( c == '0' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 6;
    }
    else if( ispunct( c ) )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 22;
    }
    else return -1;
}

int forwardSlash( Input * input )
{
    int c = advance( input );

    //if( isspace( c ) ) c = advance( input );

    if( c == '/' ) return 9;
    else if( c == '*' ) return 2;

    input->used--;

    return 46;
}

int comment( Input * input )
{
    int c = advance( input );

    if( c == '\n' ) return 0;
    return 9;
}

int blockComment( Input * input )
{
    char c = advance( input );

    if( c == '*' ) return 10;

    if( c == '/' ) return 0;
    else return 2;
}

int asterisk( Input * input )
{
    int c = advance( input );

    if( c == '/' ) return -1;
    return 2;
}

int makeWord( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( !isalnum( c ) )
    {
        input->used--;
        return 8;
    }
    else appendChar( sm, ( char )c );

    return 3;
}

int stringLiteral( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '\\' ) return 11;
    else if( c == '\"' )
    {
        appendChar( sm, ( char )c );
        return 7;
    }
    else if( c == '\n' ) return 7;
    else appendChar( sm, ( char )c );

    return 4;
}

int escapeChar( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == 'a' ) appendChar( sm, '\a' );
    else if( c == 'b' ) appendChar( sm, '\b' );
    else if( c == 'f' ) appendChar( sm, '\f' );
    else if( c == 'n' ) appendChar( sm, '\n' );
    else if( c == 'r' ) appendChar( sm, '\r' );
    else if( c == 't' ) appendChar( sm, '\t' );
    else if( c == 'v' ) appendChar( sm, '\v' );
    else if( c == '\\' ) appendChar( sm, '\\' );
    else if( c == '\'' ) appendChar( sm, '\'' );
    else if( c == '\"' ) appendChar( sm, '\"' );
    else if( c == '\0' ) appendChar( sm, '\0' );

    return 4;
}

int decimalConstant( Input * input, StringMaker * sm )
{
    char c = advance( input );

    if( c == '.' ) return 12;
    else if( c == 'E' || c == 'e' )
    {
        appendChar( sm, ( char )c );
        return 20;
    }
    else if( isdigit( c ) )
    {
        appendChar( sm, ( char )c );
        return 5;
    }
    input->used--;
    return 13;
}

int zeroConstant( Input * input, StringMaker * sm )
{
    char c = advance( input );

    if( c == '.' ) return 12;
    else if( c == 'X' || c == 'x' )
    {
        appendChar( sm, ( char )c );
        return 14;
    }
    else if( isdigit( c ) )
    {
        appendChar( sm, ( char )c );
        return 15;
    }
    input->used--;
    return 13;
}

int hexConstant( Input * input, StringMaker * sm )
{
    char c = advance( input );

    if( isxdigit( c ) )
    {
        appendChar( sm, ( char )c );
        return 14;
    }
    if( isspace( c ) || c == ';' ) return 17;
    input->used--;
    return 21;
}

int octalConstant( Input * input, StringMaker * sm )
{
    char c = advance( input );

    if( isdigit( c ) )
    {
        appendChar( sm, ( char )c );
        return 15;
    }
    else if( isspace( c ) || c == ';' ) return 18;
    input->used--;
    return 21;
}

int floatConstant( Input * input, StringMaker * sm )
{
    char c = advance( input );

    if( isdigit( c ) )
    {
        appendChar( sm, ( char )c );
        return 16;
    }
    if( isspace( c ) || c == ';' ) return 19;
    input->used--;
    return 21;
}

int decimalPoint( Input * input, StringMaker * sm )
{
    appendChar( sm, ( char )input->buffer[input->used - 1] );
    return 16;
}

int sciEe( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '+' ) appendChar( sm, ( char )c );
    else if( c == '-' ) appendChar( sm, ( char )c );
    return 16;
}

int metaChar( Input * input, StringMaker * sm )
{
    int c = ( int )sm->ptr[0];

    if( c == '*' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 23;
    }
    else if( c == '+' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 24;
    }
    else if( c == '-' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 25;
    }
    else if( c == '(' || c == ')' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 26;
    }
    else if( c == '{' || c == '}' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 27;
    }
    else if( c == '[' || c == ']' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 28;
    }
    else if( c == ',' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 35;
    }
    else if( c == '.' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 39;
    }
    else if( c == ';' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 36;
    }
    else if( c == '!' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 37;
    }
    else if( c == '%' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 38;
    }
    else if( c == '>' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 29;
    }
    else if( c == '<' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 30;
    }
    else if( c == '=' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 31;
    }
    else if( c == '&' )
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 40;
    }
    {
        setupStringMaker( sm, 2048 );
        appendChar( sm, ( char )c );

        return 43;
    }
    return 21;
}

int pointerMul( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 35;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int plus( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 34;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int minus( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 33;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int parenthesis( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, "(" ) == 0 ) token->id = 32;
    else token->id = 45;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int curlyBrace( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, "{" ) == 0 ) token->id = 31;
    else token->id = 46;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int bracket( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, "[" ) == 0 ) token->id = 30;
    else token->id = 47;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int comma( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 29;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int dotOp( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 28;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int semicolon( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 27;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int logNot( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '=' ) appendChar( sm, ( char )c );
    else input->used--;

    return 42;
}

int orOp( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '|' )
    {
        appendChar( sm, ( char )c );
        return 44;
    }
    else input->used--;

    return 21;
}

int andOp( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '&' ) appendChar( sm, ( char )c );
    else input->used--;

    return 45;
}

int tildeOp( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 40;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int modulus( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 25;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int greaterThan( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '=' ) appendChar( sm, ( char )c );
    else input->used--;
    return 32;
}

int lessThan( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '=' ) appendChar( sm, ( char )c );
    else input->used--;
    return 33;
}

int equals( Input * input, StringMaker * sm )
{
    int c = advance( input );

    if( c == '=' ) appendChar( sm, ( char )c );
    else input->used--;
    return 34;
}

int slAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 17;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int mwAbsorb( Token * token, StringMaker * sm )
{
    sprintf( token->key, "%s", ( char * )sm->ptr );
    token->id = getWordID( ( char * )sm->ptr );
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int decAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 18;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int octAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 19;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int hexAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 20;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int floatAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 21;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int notAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, "!" ) == 0 ) token->id = 26;
    else token->id = 41;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int orAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 42;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int andAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, "&" ) == 0 ) token->id = 39;
    else token->id = 43;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int gtAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, ">" ) == 0 ) token->id = 22;
    else token->id = 38;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int ltAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, "<" ) == 0 ) token->id = 23;
    else token->id = 37;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int eqAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    if( strcmp( token->key, "=" ) == 0 ) token->id = 24;
    else token->id = 36;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int divAbsorb( Token * token, StringMaker * sm )
{
    strcpy( token->key, ( char * )sm->ptr );
    token->id = 44;
    token->lineNumber = lineNumber;
    destroyStringMaker( sm );

    return -1;
}

int errorAbsorb( Token * token )
{
    return -1;
}
