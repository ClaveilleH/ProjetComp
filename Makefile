# Compiler
CC = gcc

# Source files
NAME = 3
LEX_SRC = main.l
YACC_SRC = miniC.y
FUNC_SRC = functions.c

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
	yacc -d $(YACC_SRC)

# Link the object files to create the executable
$(TARGET): $(LEX_GEN) $(YACC_GEN) $(FUNC_SRC)
	$(CC) -o $(TARGET) $(FUNC_SRC) lex.yy.c y.tab.c -lfl

# Clean up build files
clean:
	rm -f $(TARGET) $(LEX_GEN) $(YACC_GEN)  y.tab.h