#ifndef M17_Parser_h
#define M17_Parser_h


#define DESCEND printf( "***DESCENT @ File : %s - Line : %d - %s()\n", __FILE__, __LINE__, __func__ )
#define ERROR() ( printf( "SYNTAX ERROR @ function : %s(), line: %d, sourceLine : %d\n", __func__, __LINE__, lookahead.lineNumber ), errorCount++ )
#define OUTCOME printf( "\nYou have %d syntax errors!\n\n", errorCount )

int parserInit();

#endif
