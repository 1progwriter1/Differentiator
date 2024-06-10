CFLAGS=-fsanitize=address,alignment -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla

COMP=g++

VECTOR_DIR=src/vector
PARSE_DIR=src/parse
BIN_TREE_DIR=src/bin_tree
DIFF_DIR=src/differentiator
LATEX_DIR=src/gen_latex
READ_FILE_DIR=src/read_file
GRAPH_DIR=src/graph_gen

LIB_OBJ_DIR=src/lib_objects
LIB_OBJ=$(wildcard $(LIB_OBJ_DIR)/*.o)

VECTOR_SRC=$(wildcard $(VECTOR_DIR)/*.cpp)
PARSE_SRC=$(wildcard $(PARSE_DIR)/*.cpp)
BIN_TREE_SRC=$(wildcard $(BIN_TREE_DIR)/*.cpp)
DIFF_SRC=$(wildcard $(DIFF_DIR)/*.cpp)
LATEX_SRC=$(wildcard $(LATEX_DIR)/*.cpp)
READ_FILE_SRC=$(wildcard $(READ_FILE_DIR)/*.cpp)
GRAPH_SRC=$(wildcard $(GRAPH_DIR)/*.cpp)

all: $(VECTOR_SRC) $(PARSE_SRC) $(BIN_TREE_SRC) $(DIFF_SRC) $(LATEX_SRC) $(READ_FILE_SRC) $(GRAPH_SRC)
	$(COMP) $(CFLAGS) $(VECTOR_SRC) $(PARSE_SRC) $(BIN_TREE_SRC) $(DIFF_SRC) $(LATEX_SRC) $(READ_FILE_SRC) $(GRAPH_SRC) $(LIB_OBJ) src/main.cpp
	rm -rf *.dSYM
