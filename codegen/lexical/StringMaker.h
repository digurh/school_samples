#ifndef StringMaker_h
#define StringMaker_h

typedef struct StringMaker {
	unsigned char *	ptr;
	unsigned int	mingrowth;
	unsigned int	memsize;
	unsigned int	space_used;
} StringMaker;

void	setupStringMaker( StringMaker *, unsigned int );
void	appendChar( StringMaker *, unsigned char );
void	destroyStringMaker( StringMaker * );

#endif
