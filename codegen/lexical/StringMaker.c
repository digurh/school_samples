#include	"StringMaker.h"
#include    "Input.h"
#include	<stdio.h>
#include	<stdlib.h>

void
setupStringMaker( StringMaker * this, unsigned int mingrowth )
{
	this->ptr = 0;
	this->mingrowth = mingrowth;
	this->memsize = 0;
	this->space_used = 0;
}

void
appendChar( StringMaker * this, unsigned char c )
{
	if ( this->memsize > this->space_used )
	{
		this->ptr[this->space_used] = c;
		this->space_used += 1;
        //printf( "0sm : %s : %d : %d\n", this->ptr, this->memsize, this->space_used );
	}
	else if ( (this->ptr = realloc( this->ptr, this->memsize + this->mingrowth )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31realloc() falied in file %s line %d.\x1b[0m", __FILE__, __LINE__ );
		this->space_used = 0;
		this->memsize = 0;
	}
	else
	{
		this->memsize += this->mingrowth;
		this->ptr[this->space_used] = c;
		this->space_used += 1;
        //printf( "1sm : %s : %d : %d\n", this->ptr, this->memsize, this->space_used );
	}
}

void
destroyStringMaker( StringMaker * this )
{
	if ( this->ptr != 0 ) free( this->ptr );
}
