#include	"DeclarationList.h"
#include	"Declaration.h"
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>

DeclarationList *
add_decl( DeclarationList * dl, Declaration * declaration )
{
	int			size;

	if ( dl == 0 )
	{
		size = sizeof( DeclarationList ) + sizeof( Declaration );
		if ( (dl = malloc( size )) == 0 )
		{
			return 0;
		}
		else
		{
            //printf( "\x1b[1;32m**ADDING NEW DECL\x1b[0m\n" );
			dl->list[0] = *declaration;
			dl->count = 1;
			return dl;
		}
	}
	else
	{
        //printf( "\x1b[1;32m**ADDING TO DECL LIST\x1b[0m\n");
		size = sizeof( DeclarationList ) + (dl->count + 1) * sizeof( Declaration );
		if ( (dl = realloc( dl, size )) == 0 )
		{
			return 0;
		}
		else
		{
			dl->list[dl->count] = *declaration;
			dl->count += 1;
			return dl;
		}
	}
}

Declaration *
lookup_decl( DeclarationList * dl, const char * identifier )
{
	int			i;

	if ( dl == 0 )
	{
		return 0;
	}
	else
	{
        //printDeclarationList( dl, 1 );
		for ( i = 0 ; i < dl->count ; i++ )
		{
			if ( strcmp( identifier, get_identifier( &dl->list[i] ) ) == 0 )
			{
				return dl->list + i;
			}
		}
		return 0;
	}
}

DeclarationList *
destroy_decl_list( DeclarationList * dl )
{
	int			i;

	if ( dl == 0 )
	{
		return 0;
	}
	else
	{
		for ( i = 0 ; i < dl->count ; i++ )
		{
            //printf( "freeing %s\n", get_identifier( &dl->list[i] ) );
			free_identifier( &dl->list[i] );
		}
		free( dl );
		return 0;
	}
}

static void
indent( int lim )
{
	int			i;

	for ( i = 0 ; i < lim ; i++ )
		printf( "\t" );
}

void
printDeclarationList( DeclarationList * dl, int ind )
{
	int			i;

	if ( dl == 0 )
	{
		indent( ind );	printf( "DeclarationList %lx is empty\n", (unsigned long)dl );
	}
	else
	{
        printf( "\x1b[1;32mStart declaration list\x1b[0m\n" );
		for ( i = 0 ; i < dl->count ; i++ )
		{
//			printf( " %s", get_identifier( dl->list+i ) );
			printDeclaration( dl->list+i, ind + 1 );
		}
        printf( "\x1b[1;32mEnd declaration list\x1b[0m\n" );
	}
}
