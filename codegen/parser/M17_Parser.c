#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "M17_Parser.h"
#include "../lexical/M17_StateSwitch.h"
#include "../lexical/M17_StateFunctions.h"
#include "../lexical/Input.h"
#include "../lexical/M17_TokenID.h"
#include "../semantic/symbol_table.h"
#include "../semantic/TypeInfo.h"
#include "../semantic/Declaration.h"
#include "../semantic/DeclarationList.h"
#include "../semantic/Result.h"
#include "../semantic/semantic.h"
#include "../codegen/LocGen.h"
#include "../codegen/generate.h"



static Token lookahead;
static int errorCount;

static int advance(), recognize(), expressionList(), optionalExpression(), expression(), expressionSuffix(),
    logicalOrExpression(), logicalOrExpressionSuffix(), logicalAndExpression(), logicalAndExpressionSuffix(),
    relationalExpression(), relationalExpressionSuffix(), additiveExpression(), additiveExpressionSuffix(),
    multiplicativeExpression(), multiplicativeExpressionSuffix(), unaryExpression(), primaryExpression(), lhs(),
    lhsSuffix(), optionalExpressionList(), lhsList(), lhsListSuffix(), expressionListSuffix(),
    modifier(), program(), declrationDefinitionList(), declarationDefinitionSuffix(), declarationDefinition(),
    scalarType(), structSuffix(), function(), functionSuffix(), type(), simpleType(), memberList(), roml(),
    member(), optModList(), modifierList(), modListSuffix(), declarationStatement(), identifierList(), roidl(),
    optParamList(), paramList(), ropl(), parameter(), optDeclarations(), declarationList(), declarationSuffix(),
    statementList(), rosl(), statement(), expressionStatement(), blockStatement(), whileStatement(),
    doWhileStatement(), forStatement(), ifStatement(), optElseStatement(), returnStatement();

//called once from driver. enables advance() to be used strictly to fetch tokens
//parserInit() grabs first token with advance(), advance() returns token, parserInit() decides point of descent, never called again
int parserInit()
{
    errorCount = 0;

    if( setup_symbol_table() != 1 ) exit( 0 );

    if( advance() == 0 ) return 0;
    else if( lookahead.id == CHAR_ID ) program();
    else if( lookahead.id == INT_ID ) program();
    else if( lookahead.id == FLOAT_ID ) program();
    else if( lookahead.id == STRUCT_ID ) program();
    OUTCOME;
    return 1;
}

//contains mechanism to retrieve next token
//possible implementation:
//  1. driver gets token and passes it to parser, parser returns something to driver to alert it for next token
//  2. probably better: advance() gets token directly. driver still does setup, but passes off while loop function to advance()
int advance()
{
    if( getToken( &lookahead ) == 0 ) return 0;
    //printf( "Getting token : %s\n", lookahead.key );
    return 1;
}

int recognize( int id )
{
    ////DESCEND;
    if( lookahead.id == id )
    {
        advance();
        return 1;
    }
    return 0;
}

int program()
{
    //DESCEND;
    Declaration decl;
    DeclarationList * dl = 0;

    add_decl( dl, &decl );
    enter_scope();

    if( declrationDefinitionList() == 1 )
    {
        exit_scope();

        return 1;
    }
    return 0;
}

int declrationDefinitionList()
{
    //DESCEND;
    if( declarationDefinition() == 0 ) return 0;
    else if( declarationDefinitionSuffix() == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int declarationDefinitionSuffix()
{
    //DESCEND;
    if( declarationDefinition() == 0 ) return 1;
    else if( declarationDefinitionSuffix() == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int declarationDefinition()
{
    //DESCEND;
    Declaration decl;
    Type ty;

    if( recognize( VOID_ID ) == 1 && function( &ty ) == 1 ) return 1;
    else if( scalarType( &ty ) == 1 && optModList( &ty ) == 1 && function( &ty ) == 1 ) return 1;
    else if( recognize( STRUCT_ID ) == 1 )
    {
        char * struct_name = malloc( sizeof( lookahead.key ) );
        strcpy( struct_name, lookahead.key );

        if( recognize( IDENTIFIER_ID ) == 1 )
        {
            decl = *add_struct( struct_name );
            ty.typeinfo = make_struct_type( struct_name, &ty );

            if( structSuffix( &decl ) == 1 ) return 1;
            else
            {
                ERROR();
                return 0;
            }
        }
        else
        {
            ERROR();
            return 0;
        }
    }
    return 0;
}

int scalarType( Type * ty )
{
    //DESCEND;
    if( recognize( CHAR_ID ) == 1 )        // scalar_type ::= char
    {
        ty->typeinfo = make_builtin_type( type_char, ty );
        return 1;
    }
    else if( recognize( INT_ID ) == 1 )     // scalar_type ::= int
    {
        ty->typeinfo = make_builtin_type( type_int, ty );
        return 1;
    }
    else if( recognize( FLOAT_ID ) == 1 )     // scalar_type ::= float
    {
        ty->typeinfo = make_builtin_type( type_float, ty );
        return 1;
    }
    return 0;
}

int structSuffix( Declaration * decl )
{
    //DESCEND;
    Type ty;
    LocGen lg;

    if( recognize( L_CURLY_BRACE_ID ) == 1 )
    {
        decl = enter_struct( decl );
        setupLocGen( &lg, 1 );
        if( memberList( &lg ) == 1 )
        {
            exit_struct();
            if( recognize( R_CURLY_BRACE_ID ) == 1 ) return 1;
        }
    }
    else if( optModList( &ty ) == 1 && function( &ty ) == 1 ) return 1;
    return 0;
}

int function( Type * ty )
{
    //DESCEND;
    Declaration decl;
    char * func_name = malloc( sizeof( lookahead.key ) );
    strcpy( func_name, lookahead.key );

    if( ( decl = *add_function( func_name, ty ) ), recognize( IDENTIFIER_ID ) == 0 ) return 0;
    else if( ( decl = *enter_function( &decl ) ), recognize( L_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( optParamList() == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( R_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( functionSuffix() == 0 )
    {
        ERROR();
        return 0;
    }
    exit_function();
    //destroy_decl_list( dl );

    return 1;
}

int functionSuffix()
{
    //DESCEND;
    LocGen lg;
    setupLocGen( &lg, 2 );

    if( recognize( SEMICOLON_ID ) == 1 ) return 1;
    else if( blockStatement( &lg ) == 1 ) return 1;
    return 0;
}

int type( Type * ty )
{
    //DESCEND;
    if( simpleType( ty ) == 0 ) return 0;
    else if( optModList( ty ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int simpleType( Type * ty )
{
    //DESCEND;
    if( scalarType( ty ) == 1 ) return 1;
    else if( recognize( STRUCT_ID ) == 1 )
    {
        char * struct_ty = malloc( sizeof( lookahead.key ) );
        strcpy( struct_ty, lookahead.key );

        if( recognize( IDENTIFIER_ID ) == 1 )
        {
            add_struct( struct_ty );
            ty->typeinfo = make_struct_type( struct_ty, ty );
            return 1;
        }
        else
        {
            ERROR();
            return 0;
        }
    }
    return 0;
}

int memberList( LocGen * lg )
{
    //DESCEND;
    if( member( lg ) == 0 ) return 0;
    else if( roml( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int roml( LocGen * lg )
{
    //DESCEND;
    if( member( lg ) == 1 && roml( lg ) == 1 ) return 1;
    return 1;
}

int member( LocGen * lg )
{
    //DESCEND;
    Type ty;

    if( type( &ty ) == 0 ) return 0;
    else if( identifierList( &ty, lg, 2 ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( SEMICOLON_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int optModList( Type * ty )
{
    //DESCEND;
    if( modifierList( ty ) == 1 ) return 1;
    return 1;
}

int modifierList( Type * ty )
{
    //DESCEND;
    if( modifier( ty ) == 0 ) return 0;
    else if( modListSuffix( ty ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int modListSuffix( Type * ty )
{
    //DESCEND;
    if( modifier( ty ) == 0 ) return 1;
    else if( modListSuffix( ty ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int modifier( Type * ty )
{
    //DESCEND;
    char * array_cap = malloc( sizeof( lookahead.key ) );

    if( recognize( L_BRACKET_ID ) == 1 )
    {
        if( strcpy( array_cap, lookahead.key ), recognize( DEC_CONST_ID ) == 1 && recognize( R_BRACKET_ID ) == 1 )
        {
            int ac = atoi( array_cap );
            ty->typeinfo = add_array( ty, ac );
            return 1;
        }
        else
        {
            ERROR();
            return 0;
        }
    }
    else if( recognize( PTR_MUL_ID ) == 1 ) return 1;
    return 0;
}

int declarationStatement( LocGen * lg )
{
    //DESCEND;
    Type ty;

    if( type( &ty ) == 0 ) return 0;
    else if( identifierList( &ty, lg, 0 ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( SEMICOLON_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

////////////////////
void add_thing( char * identifier, Type * ty, LocGen * lg, int flag )
{
    char location[40];

    switch( flag )
    {
        case 0:
            get_location( lg, sizeof( ty ), location );
            add_variable( identifier, ty, location );
            break;
        case 1:
            get_location( lg, sizeof( ty ), location );
            add_parameter( identifier, ty, location );
            break;
        case 2:
            get_location( lg, sizeof( ty ), location );
            add_member( identifier, ty, location );
            break;
        default:
            break;
    }
}
///////////////////

int identifierList( Type * ty, LocGen * lg, int flag )
{
    //DESCEND;
    char * id = malloc( sizeof( lookahead.key ) );
    strcpy( id, lookahead.key );

    if( recognize( IDENTIFIER_ID ) == 0 ) return 0;
    else if( add_thing( id, ty, lg, flag ), roidl( ty, lg, flag ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int roidl( Type * ty, LocGen * lg, int flag )
{
    //DESCEND;
    char * id = malloc( sizeof( lookahead.key ) );
    if( recognize( COMMA_ID ) == 0 ) return 1;
    else if( strcpy( id, lookahead.key ), recognize( IDENTIFIER_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( add_thing( id, ty, lg, flag ), roidl( ty, lg, flag ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int optParamList()
{
    //DESCEND;
    if( paramList() == 1 ) return 1;
    return 1;
}

int paramList()
{
    //DESCEND;
    LocGen lg;
    setupLocGen( &lg, 3 );

    if( parameter( &lg ) == 0 ) return 0;
    else if( ropl( &lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int ropl( LocGen * lg )
{
    //DESCEND;
    if( recognize( SEMICOLON_ID ) == 0 ) return 1;
    else if( parameter( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( ropl( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int parameter( LocGen * lg )
{
    //DESCEND;
    Type ty;

    if( type( &ty ) == 0 ) return 0;
    else if( identifierList( &ty, lg, 1 ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int optDeclarations( LocGen * lg )
{
    //DESCEND;
    if( declarationList( lg ) == 0 ) return 1;
    return 1;
}

int declarationList( LocGen * lg )
{
    //DESCEND;
    if( declarationStatement( lg ) == 0 ) return 0;
    else if( declarationSuffix( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int declarationSuffix( LocGen * lg )
{
    //DESCEND;
    if( declarationStatement( lg ) == 0 ) return 1;
    else if( declarationSuffix( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int statementList( LocGen * lg )
{
    //DESCEND;
    if( statement( lg ) == 0 ) return 0;
    else if( rosl( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int rosl( LocGen * lg )
{
    //DESCEND;
    if( statement( lg ) == 0 ) return 1;
    else if( rosl( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int statement( LocGen * lg)
{
    //DESCEND;
    Result result;

    if( expressionStatement( &result ) == 1 ) return 1;
    else if( blockStatement( lg ) == 1 ) return 1;
    else if( whileStatement( &result ) == 1 ) return 1;
    else if( doWhileStatement( &result ) == 1 ) return 1;
    else if( forStatement( &result ) == 1 ) return 1;
    else if( ifStatement( &result ) == 1 ) return 1;
    else if( returnStatement( &result ) == 1 ) return 1;
    return 0;
}

int expressionStatement( Result * result )
{
    //DESCEND;
    if( expression( result ) == 0 ) return 0;
    else if( recognize( SEMICOLON_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int blockStatement( LocGen * lg )
{
    //DESCEND;
    if( recognize( L_CURLY_BRACE_ID ) == 0 ) return 0;
    else if( enter_scope(), optDeclarations( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( statementList( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( exit_scope(), recognize( R_CURLY_BRACE_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int whileStatement( Result * result, LocGen * lg  )
{
    //DESCEND;
    if( recognize( WHILE_ID ) == 0 ) return 0;
    else if( recognize( L_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( expression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( R_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( statement( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int doWhileStatement( Result * result, LocGen * lg  )
{
    //DESCEND;
    if( recognize( DO_ID ) == 0 ) return 0;
    else if( statement( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( WHILE_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( L_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( expression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( R_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int forStatement( Result * result, LocGen * lg  )
{
    //DESCEND;
    if( recognize( FOR_ID ) == 0 ) return 0;
    else if( recognize( L_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( optionalExpression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( SEMICOLON_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( optionalExpression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( SEMICOLON_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( optionalExpression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( R_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( statement( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int ifStatement( Result * result, LocGen * lg  )
{
    //DESCEND;
    if( recognize( IF_ID ) == 0 ) return 0;
    else if( recognize( L_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( expression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( R_PAREN_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( statement( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( optElseStatement( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int optElseStatement( LocGen * lg )
{
    //DESCEND;
    if( recognize( ELSE_ID ) == 0 ) return 1;
    else if( statement( lg ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int returnStatement( Result * result )
{
    //DESCEND;
    if( recognize( RETURN_ID ) == 0 ) return 0;
    else if( optionalExpression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( recognize( SEMICOLON_ID ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int optionalExpression( Result * output )
{
    if( expression( output ) == 0 ) return 1;
    return 1;
}

int expression( Result * output )
{
    //DESCEND;
    Result leftopnd;

    if( logicalOrExpression( &leftopnd ) == 0 ) return 0;
    else if( expressionSuffix( &leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int expressionSuffix( const Result * leftopnd, Result * output )
{
    //DESCEND;
    //Result temp;

    if( recognize( GETS_ID ) == 0 )
    {
        *output = *leftopnd;
        return 1;
    }
    else if( expression( output ) == 0 )
    {
        ERROR();
        return 0;
    }
    else
    {
        *output = *leftopnd;
        return 1;
    }
}

int logicalOrExpression( Result * output )
{
    //DESCEND;
    Result leftopnd;

    if( logicalAndExpression( &leftopnd ) == 0 ) return 0;
    else if( logicalOrExpressionSuffix( &leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int logicalOrExpressionSuffix( const Result * leftopnd, Result * output )
{
    //DESCEND;
    //Result temp;

    if( recognize( LOG_NOT_ID ) == 0 )
    {
        *output = *leftopnd;
        return 1;
    }
    else if( relationalExpression( output ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( logicalOrExpressionSuffix( leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    else
    {
        *output = *leftopnd;
        return 1;
    }
}

int logicalAndExpression( Result * output )
{
    //DESCEND;
    Result leftopnd;

    if( relationalExpression( &leftopnd ) == 0 ) return 0;
    else if( logicalAndExpressionSuffix( &leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int logicalAndExpressionSuffix( const Result * leftopnd, Result * output )
{
    //DESCEND;
    //Result temp;

    if( recognize( LOG_AND_ID ) == 0 )
    {
        *output = *leftopnd;
        return 1;
    }
    else if( relationalExpression( output ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( logicalAndExpressionSuffix( leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    else
    {
        *output = *leftopnd;
        return 1;
    }
}

int relationalExpression( Result * output )
{
    //DESCEND;
    Result leftopnd;

    if( additiveExpression( &leftopnd ) == 0 ) return 0;
    else if( relationalExpressionSuffix( &leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int relationalExpressionSuffix( const Result * leftopnd, Result * output )
{
    //DESCEND;
    //Result temp;

    if( recognize( EQ_ID ) == 1 && additiveExpression( output ) == 1 ) return 1;
    else if( recognize( NOT_EQ_ID ) == 1 && additiveExpression( output ) == 1 ) return 1;
    else if( recognize( GT_ID ) == 1 && additiveExpression( output ) == 1 ) return 1;
    else if( recognize( LT_ID ) == 1 && additiveExpression( output ) == 1 ) return 1;
    else if( recognize( GT_EQ_ID ) == 1 && additiveExpression( output ) == 1 ) return 1;
    else if( recognize( LT_EQ_ID ) == 1 && additiveExpression( output ) == 1 ) return 1;
    else
    {
        *output = *leftopnd;
        return 1;
    }
}

int additiveExpression( Result * output )
{
    //DESCEND;
    Result leftopnd;

    if( multiplicativeExpression( &leftopnd ) == 0 ) return 0;
    else if( additiveExpressionSuffix( &leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int additiveExpressionSuffix( const Result * leftopnd, Result * output )
{
    //DESCEND;
    Result temp;

    if( recognize( PLUS_ID ) == 1 )
    {
        generate( 0, "leaq", leftopnd->location, "(%rax)", "created from additiveExpressionSuffix line 924" );
        generate( 0, "movq", "(%rax)", "-90(%rbp)", "created from additiveExpressionSuffix line 925" );

        if( multiplicativeExpression( output ) == 0 )
        {
            ERROR();
            return 0;
        }
        else
        {
            generate( 0, "movq", output->location, "(%rdx)", "created from additiveExpressionSuffix" );
            generate( 0, "lmovq", output->location, "(%rdx)", "created from additiveExpressionSuffix" );
            generate( 0, "addq", "-90(%rbp)", "(%rdx)", "created from additiveExpressionSuffix" );
            temp.flags = IS_LVALUE;
            temp.type = leftopnd->type;
            strcpy( temp.location, "(%rdx)" );

            return additiveExpressionSuffix( &temp, output );
        }
    }
    else if( recognize( MINUS_ID ) == 1 )
    {

        generate( 0, "leaq", leftopnd->location, "(%rax)", "created from madditiveExpressionSuffix" );
        generate( 0, "movq", "(%rax)", "-90(%rbp)", "created from additiveExpressionSuffix" );

        if( multiplicativeExpression( output ) == 0 )
        {
            ERROR();
            return 0;
        }
        else
        {
            generate( 0, "movq", output->location, "(%rdx)", "created from additiveExpressionSuffix" );
            generate( 0, "lmovq", output->location, "(%rdx)", "created from additiveExpressionSuffix" );
            generate( 0, "addq", "-90(%rbp)", "(%rdx)", "created from additiveExpressionSuffix" );
            temp.flags = IS_LVALUE;
            temp.type = leftopnd->type;
            strcpy( temp.location, "(%rdx)" );

            return additiveExpressionSuffix( &temp, output );
        }
    }
    else
    {
        *output = *leftopnd;
        return 1;
    }
}

int multiplicativeExpression( Result * output )
{
    //DESCEND;
    Result leftopnd;

    if( unaryExpression( &leftopnd ) == 0 ) return 0;
    else if( multiplicativeExpressionSuffix( &leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int multiplicativeExpressionSuffix( const Result * leftopnd, Result * output )
{
    //DESCEND;
    Result temp;

    if( recognize( PTR_MUL_ID ) == 1 )
    {
        generate( 0, "leaq", leftopnd->location, "(%rax)", "created from multiplicativeExpressionSuffix" );
        generate( 0, "movq", "(%rax)", "-90(%rbp)", "created from multiplicativeExpressionSuffix" );

        if( unaryExpression( output ) == 0 )
        {
            ERROR();
            return 0;
        }
        else
        {
            generate( 0, "movq", output->location, "(%rdx)", "created from multiplicativeExpressionSuffix" );
            generate( 0, "lmovq", output->location, "(%rdx)", "created from multiplicativeExpressionSuffix" );
            generate( 0, "mulq", "-90(%rbp)", "(%rdx)", "created from multiplicativeExpressionSuffix" );
            temp.flags = IS_LVALUE;
            temp.type = leftopnd->type;
            strcpy( temp.location, "(%rdx)" );

            return multiplicativeExpressionSuffix( &temp, output );
        }
    }
    else if( recognize( DIVIDE_ID ) == 1 && unaryExpression( output ) == 1 && multiplicativeExpressionSuffix( leftopnd, output ) == 1 ) return 1;
    else if( recognize( MOD_ID ) == 1 && unaryExpression( output ) == 1 && multiplicativeExpressionSuffix( leftopnd, output ) == 1 ) return 1;
    else
    {
        *output = *leftopnd;
        return 1;
    }
}

int unaryExpression( Result * output )
{
    //DESCEND;
    //Result leftopnd;

    if( recognize( MINUS_ID ) == 1 && primaryExpression( output ) == 1 ) return 1;
    else if( recognize( LOG_NOT_ID ) == 1 && primaryExpression( output ) == 1 ) return 1;
    else if( recognize( AND_ID ) == 1 && primaryExpression( output ) == 1 ) return 1;
    else if( recognize( TILDE_ID ) == 1 && primaryExpression( output ) == 1 ) return 1;
    else if( recognize( PTR_MUL_ID ) == 1 && primaryExpression( output ) == 1 ) return 1;
    else if( primaryExpression( output ) == 1 ) return 1;
    return 0;
}

int primaryExpression( Result * output )
{
    //DESCEND;
    //Result leftopnd;
    Type ty;

    char * lexeme = malloc( sizeof( lookahead.key ) );

    if( lhs( output ) == 1 ) return 1;
    else if( recognize( READ_ID ) == 1 && recognize( L_PAREN_ID ) == 1 && lhsList( output ) == 1 && recognize( R_PAREN_ID ) == 1 ) return 1;
    else if( recognize( WRITE_ID ) == 1 && recognize( L_PAREN_ID ) == 1 && expressionList( output ) == 1 && recognize( R_PAREN_ID ) == 1 ) return 1;
    else if( recognize( DEC_CONST_ID ) == 1 )
    {
        output->flags = 0;
        output->type = make_builtin_type( type_int, &ty );
        strcpy( output->location, lexeme );
        return 1;
    }
    else if( recognize( FLOAT_CONST_ID ) == 1 ) return 1;
    else if( recognize( HEX_CONST_ID ) == 1 )
    {
        output->flags = 0;
        output->type = make_builtin_type( type_int, &ty );
        strcpy( output->location, lexeme );
        return 1;
    }
    else if( recognize( OCT_CONST_ID ) == 1 )
    {
        output->flags = 0;
        output->type = make_builtin_type( type_int, &ty );
        strcpy( output->location, lexeme );
        return 1;
    }
    else if( recognize( STRING_LIT_ID ) == 1 ) return 1;
    else if( recognize( L_PAREN_ID ) == 1 && expression( output ) == 1 && recognize( R_PAREN_ID ) == 1 ) return 1;
    return 0;
}
//******************************
void set_result( Declaration * decl, Result * result, int flag )
{
    //printf( "\x1b[1;36m    Setting result*************\x1b[0m\n" );
    result->flags = flag;
    result->type = get_type_info( decl );
    strcpy( result->location, get_decl_location( decl ) );
}
//*******************************
int lhs( Result * output )
{
    //DESCEND;
    char * lo = malloc( sizeof( lookahead.key ) );
    strcpy( lo, lookahead.key );

    Result leftopnd;

    if( recognize( IDENTIFIER_ID ) == 0 ) return 0;
    else if( set_result( lookup_declaration( lo ), &leftopnd, 0x04 ), lhsSuffix( &leftopnd, output ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int lhsSuffix( const Result * leftopnd, Result * output )
{
    //DESCEND;
    Result rightopnd, temp;

    if( recognize( L_BRACKET_ID ) == 1 )
    {
        char * multiplier = malloc( sizeof( lookahead.key ) );
        strcpy( multiplier, lookahead.key );

        generate( 0, "leaq", leftopnd->location, "(%rax)", "created from lhsSuffix" );
        generate( 0, "movq", "(%rax)", "-90(%rbp)", "created from lhsSuffix" );

        if( expression( &rightopnd ) == 0 )
        {
            ERROR();
            return 0;
        }
        else if( recognize( R_BRACKET_ID ) == 0 )
        {
            ERROR();
            return 0;
        }
        else
        {
            get_multiplier( leftopnd->type, rightopnd.type, multiplier );
            strcat( multiplier,  "(%rax)" );
            generate( 0, "movslq", rightopnd.location, "(%rdx)", "created from lhsSuffix" );
            generate( 0, "lmovq", multiplier, "(%rdx)", "created from lhsSuffix" );
            generate( 0, "addq", "-90(%rbp)", "(%rdx)", "created from lhsSuffix" );
            temp.flags = IS_LVALUE;
            temp.type = remove_array( leftopnd->type );
            strcpy( temp.location, "(%rdx)" );

            return lhsSuffix( &temp, output );
        }
    }
    else if( recognize( DOT_OP_ID ) == 1 && recognize( IDENTIFIER_ID ) == 1 && lhsSuffix( leftopnd, output ) == 1 ) return 1;
    else if( recognize( L_PAREN_ID ) == 1 && optionalExpressionList( output ) == 1 && recognize( R_PAREN_ID ) == 1 ) return 1;
    else
    {
        *output = *leftopnd;              // lhsSuffix ::= empty
        return 1;
    }
}

int lhsList( Result * result )
{
    //DESCEND;
    if( lhs( result ) == 0 ) return 0;
    else if( lhsListSuffix( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int lhsListSuffix( Result * result )
{
    //DESCEND;
    if( recognize( COMMA_ID ) == 0 ) return 1;
    else if( lhs( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( lhsListSuffix( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int expressionList( Result * result )
{
    //DESCEND;
    if( expression( result ) == 0 ) return 0;
    else if( expressionListSuffix() == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int expressionListSuffix( Result * result )
{
    //DESCEND;
    if( recognize( COMMA_ID ) == 0 ) return 1;
    else if( expression( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    else if( expressionListSuffix( result ) == 0 )
    {
        ERROR();
        return 0;
    }
    return 1;
}

int optionalExpressionList( Result * result )
{
    //DESCEND;
    if( expressionList( result ) == 0 ) return 1;
    return 1;
}
