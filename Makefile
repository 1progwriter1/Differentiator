CFLAGS=-fsanitize=address,alignment -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla

COMP=g++

OBJ_DIR=object_files
LIB_DIR=libs_objects
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
OBJ_MOVED=$(wildcard $(OBJ_DIR)/*.o)
LIB_OBJ=$(wildcard $(LIB_DIR)/*.o)


all: $(SOURCES)
	$(COMP) $(CFLAGS) -c $(SOURCES)
	mv $(OBJECTS) $(OBJ_DIR)/
	$(COMP) $(CFLAGS) $(OBJ_MOVED) $(LIB_OBJ)

clean:
	rm -f $(OBJ_MOVED)