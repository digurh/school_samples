CC = gcc
LEXICAL = lexical
PARSER = parser
SEMANTIC = semantic
CODEGEN = codegen
FLAGS = -g -Wall -I/$(LEXICAL) -I/$(PARSER) -I/$(SEMANTIC) -I/$(CODEGEN)
COMPILE = $(CC) $(FLAGS)
M17 = m17

M17_SOURCES = $(LEXICAL_SOURCES) $(PARSER_SOURCES) $(SEMANTIC_SOURCES) $(CODEGEN_SOURCES)
LEXICAL_SOURCES = $(LEXICAL)/M17_Driver.c $(LEXICAL)/M17_StateSwitch.c $(LEXICAL)/M17_StateFunctions.c $(LEXICAL)/M17_TokenID.c $(LEXICAL)/Input.c $(LEXICAL)/StringMaker.c
PARSER_SOURCES = $(PARSER)/M17_Parser.c
SEMANTIC_SOURCES = $(SEMANTIC)/DeclarationList.c $(SEMANTIC)/Declaration.c $(SEMANTIC)/symbol_table.c $(SEMANTIC)/semantic.c $(SEMANTIC)/TypeInfo.c
CODEGEN_SOURCES = $(CODEGEN)/LocGen.c $(CODEGEN)/TempMgr.c $(CODEGEN)/generate.c $(CODEGEN)/int_stack.c


all: $(M17)
#$(LEXICAL).o $(PARSER).o $(SEMANTIC).o

$(M17): $(M17_SOURCES)
	$(COMPILE) $(M17_SOURCES) -o $(M17)

$(LEXICAL).o: $(LEXICAL_SOURCES)
	$(COMPILE) -c $(LEXICAL_SOURCES)

$(PARSER).o: $(PARSER_SOURCES)
	$(COMPILE) -c $(PARSER_SOURCES)

$(SEMANTIC).o: $(SEMANTIC_SOURCES)
	$(COMPILE) -c $(SEMANTIC_SOURCES)

$(CODEGEN).o: $(CODEGEN_SOURCES)
	$(COMPILE) -c $(CODEGEN_SOURCES)

test:
	./m17 test_0/test3.m17

clean:
	rm -f *.o
	rm -f $(M17)
