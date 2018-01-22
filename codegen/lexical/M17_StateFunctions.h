#ifndef M17_StateFunctions_h
#define M17_StateFunctions_h


#include "Input.h"
#include "StringMaker.h"
#include "M17_TokenID.h"

typedef struct Token
{
    char key[2048];
    ID id;
    unsigned int lineNumber;
} Token;


int zeroState( Input * input, StringMaker * sm );
int forwardSlash( Input * input );
int comment( Input * input );
int blockComment( Input * input );
int asterisk( Input * input );
int makeWord( Input * input, StringMaker * sm );
int stringLiteral( Input * input, StringMaker * sm );
int escapeChar( Input * input, StringMaker * sm );
int decimalConstant( Input * input, StringMaker * sm );
int zeroConstant( Input * input, StringMaker * sm );
int hexConstant( Input * input, StringMaker * sm );
int octalConstant( Input * input, StringMaker * sm );
int floatConstant( Input * input, StringMaker * sm );
int decimalPoint( Input * input, StringMaker * sm );
int sciEe( Input * input, StringMaker * sm );
int metaChar( Input * input, StringMaker * sm );
int pointerMul( Token * token, StringMaker * sm );
int plus( Token * token, StringMaker * sm );
int minus( Token * token, StringMaker * sm );
int parenthesis( Token * token, StringMaker * sm );
int curlyBrace( Token * token, StringMaker * sm );
int bracket( Token * token, StringMaker * sm );
int comma( Token * token, StringMaker * sm );
int dotOp( Token * token, StringMaker * sm );
int semicolon( Token * token, StringMaker * sm );
int logNot( Input * input, StringMaker * sm );
int orOp( Input * input, StringMaker * sm );
int andOp( Input * input, StringMaker * sm );
int tildeOp( Token * token, StringMaker * sm );
int modulus( Token * token, StringMaker * sm );
int greaterThan( Input * input, StringMaker * sm );
int lessThan( Input * input, StringMaker * sm );
int equals( Input * input, StringMaker * sm );
int slAbsorb( Token * token, StringMaker * sm );
int mwAbsorb( Token * token, StringMaker * sm );
int decAbsorb( Token * token, StringMaker * sm );
int hexAbsorb( Token * token, StringMaker * sm );
int octAbsorb( Token * token, StringMaker * sm );
int floatAbsorb( Token * token, StringMaker * sm );
int notAbsorb( Token * token, StringMaker * sm );
int orAbsorb( Token * token, StringMaker * sm );
int andAbsorb( Token * token, StringMaker * sm );
int gtAbsorb( Token * token, StringMaker * sm );
int ltAbsorb( Token * token, StringMaker * sm );
int eqAbsorb( Token * token, StringMaker * sm );
int divAbsorb( Token * token, StringMaker * sm );
int errorAbsorb( Token * token );


#endif
