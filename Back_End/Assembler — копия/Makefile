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

asmtask: main.o assembler.o text_functions.o
	g++ ./asm_obj/main.o ./asm_obj/assembler.o ./asm_obj/text_functions.o $(FLAGS) -o assembly

main.o: ./asm_src/main.cpp
	g++ -c asm_src/main.cpp $(FLAGS) -o asm_obj/main.o

assembler.o: ./asm_src/assembler.cpp
	g++ -c asm_src/assembler.cpp $(FLAGS) -o asm_obj/assembler.o

text_functions.o:
	g++ -c ./Onegin/text_functions.cpp $(FLAGS) -o ./asm_obj/text_functions.o

preproc: ./asm_src/assembler.cpp
	g++ -c asm_src/assembler.cpp $(FLAGS) -E -o asm_obj/preproc.cpp

.PHONY: clean

clean:
	rm *.o assembly