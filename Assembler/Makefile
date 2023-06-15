all: asmtask

FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++\
 -Wc++14-compat -Wmissing-declarations\
  -Wcast-align -Wcast-qual -Wchar-subscripts\
   -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal\
    -Wformat-nonliteral -Wformat-security -Wformat=2\
	 -Winline -Wlong-long -Wnon-virtual-dtor -Wopenmp\
	 -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow\
	   -Wsign-conversion -Wsign-promo -Wstrict-overflow=2\
		 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef\
		  -Wunreachable-code -Wunused -Wvariadic-macros\
		   -Wno-literal-range -Wno-missing-field-initializers -Wno-narrowing\
		    -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new\
			 -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer\
			  -Wlarger-than=8192 -fPIE\
			   -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,\
			   -fsanitize=float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,\
			   -fsanitize=address,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,

asmtask: main.o assembler.o parser.o
	g++ ./asm_obj/main.o ./asm_obj/assembler.o ./asm_obj/parser.o $(FLAGS) -o assembly

main.o: main.cpp
	g++ -c main.cpp $(FLAGS) -o asm_obj/main.o

assembler.o: assembler.cpp
	g++ -c assembler.cpp $(FLAGS) -o asm_obj/assembler.o

parser.o:
	g++ -c ../Parsering/parser.cpp $(FLAGS) -o ./asm_obj/parser.o

.PHONY: clean

clean:
	rm *.o assembly