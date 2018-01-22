#ifndef M17_TokenID_h
#define M17_TokenID_h


typedef enum ID { CHAR_ID = 0, INT_ID = 1, FLOAT_ID = 2, STRUCT_ID = 3, VOID_ID = 4,
        WHILE_ID = 5, DO_ID = 6, FOR_ID = 7, IF_ID = 8, ELSE_ID = 9, RETURN_ID = 10,
        READ_ID = 11, WRITE_ID = 12, ALLOCATE_ID = 13, DEALLOCATE_ID = 14, FUNC_ID = 15,
        IDENTIFIER_ID = 16, STRING_LIT_ID = 17, DEC_CONST_ID = 18, OCT_CONST_ID = 19,
        HEX_CONST_ID = 20, FLOAT_CONST_ID = 21, GT_ID = 22, LT_ID = 23, GETS_ID = 24,
        MOD_ID = 25, LOG_NOT_ID = 26, SEMICOLON_ID = 27, DOT_OP_ID = 28, COMMA_ID = 29,
        L_BRACKET_ID = 30, L_CURLY_BRACE_ID = 31, L_PAREN_ID = 32, MINUS_ID = 33, PLUS_ID = 34,
        PTR_MUL_ID = 35, EQ_ID = 36, LT_EQ_ID = 37, GT_EQ_ID = 38, AND_ID = 39, TILDE_ID = 40,
        NOT_EQ_ID = 41, OR_ID = 42, LOG_AND_ID = 43, DIVIDE_ID = 44, R_PAREN_ID = 45,
        R_CURLY_BRACE_ID = 46, R_BRACKET_ID = 47 }
        ID;


ID getWordID( char * word );

#endif
