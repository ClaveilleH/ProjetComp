# Compiler
CC = gcc

# Source files
LEX_SRC = main.l
YACC_SRC = miniC.y
C_SOURCES = symboles.c genererDot.c

# Generated files
LEX_GEN = lex.yy.c
YACC_GEN = y.tab.c
YACC_HEADER = y.tab.h

# Output executable
TARGET = comp.out

# Default target
all: $(TARGET)

# Generate lexer source from lex
$(LEX_GEN): $(LEX_SRC)
	lex $(LEX_SRC)

# Generate parser source and header from yacc
$(YACC_GEN) $(YACC_HEADER): $(YACC_SRC)
	yacc -d -v $(YACC_SRC)

# Build the final executable
$(TARGET): $(LEX_GEN) $(YACC_GEN) $(C_SOURCES)
	$(CC) -o $(TARGET) $(LEX_GEN) $(YACC_GEN) $(C_SOURCES) -lfl

# Clean up generated and built files
clean:
	rm -f $(TARGET) $(LEX_GEN) $(YACC_GEN) $(YACC_HEADER) y.output
