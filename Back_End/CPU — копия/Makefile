all: cputask

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

cputask: main.o cpu.o stack.o debug.o
	g++ cpu_obj/main.o cpu_obj/cpu.o cpu_obj/stack.o cpu_obj/debug.o $(FLAGS) -o cpuiu

main.o: ./cpu_src/main.cpp
	g++ -c ./cpu_src/main.cpp $(FLAGS) -o cpu_obj/main.o

cpu.o: ./cpu_src/cpu.cpp
	g++ -c ./cpu_src/cpu.cpp $(FLAGS) -o cpu_obj/cpu.o

stack.o: ./Stack/src/stack.cpp
	g++ -c ./Stack/src/stack.cpp $(FLAGS) -o cpu_obj/stack.o

debug.o: ./Stack/src/debug.cpp
	g++ -c ./Stack/src/debug.cpp $(FLAGS) -o cpu_obj/debug.o

preproc: ./cpu_src/cpu.cpp
	g++ -c cpu_src/cpu.cpp $(FLAGS) -E -o cpu_obj/preproc.cpp

.PHONY: clean

clean:
	rm *.o cpuiu