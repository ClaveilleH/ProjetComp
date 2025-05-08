# Compiler
CC = gcc

# Source files
LEX_SRC = main.l
YACC_SRC = miniC.y
SYMBOL_SRC = symboles.c

# Generated files
LEX_GEN = lex.yy.c
YACC_GEN = y.tab.c y.tab.h



# Output executable
TARGET = comp.out

# Default target
all: $(TARGET)

# Generate the C source files from lex and yacc
$(LEX_GEN): $(LEX_SRC)
	lex $(LEX_SRC)

$(YACC_GEN): $(YACC_SRC)
	yacc -d -v $(YACC_SRC)

# Link the object files to create the executable
$(TARGET): $(YACC_GEN) $(LEX_GEN)  $(SYMBOL_SRC)
	$(CC) -o $(TARGET) $(FUNC_SRC) lex.yy.c y.tab.c $(SYMBOL_SRC) -lfl

# Clean up build files
clean:
	rm -f $(TARGET) $(LEX_GEN) $(YACC_GEN)  y.tab.h