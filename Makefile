all: difftask

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

difftask: main.o tree.o dump.o parser.o frontend.o translator.o lexer.o back_end.o
	g++ obj/main.o obj/tree.o obj/dump.o obj/parser.o obj/frontend.o obj/translator.o obj/lexer.o obj/back_end.o $(FLAGS) -o diff

main.o: ./main.cpp
	g++ -c ./main.cpp $(FLAGS) -o obj/main.o

tree.o: ./Tree/tree.cpp
	g++ -c ./Tree/tree.cpp $(FLAGS) -o obj/tree.o

dump.o: ./Tree/dump.cpp
	g++ -c ./Tree/dump.cpp $(FLAGS) -o obj/dump.o

parser.o: ./Parsering/parser.cpp
	g++ -c ./Parsering/parser.cpp $(FLAGS) -o obj/parser.o

# front_end.o: ./Front_End/front_end.cpp
# 	g++ -c ./Front_End/front_end.cpp $(FLAGS) -o obj/front_end.o

frontend.o: ./Front_End/frontend.cpp
	g++ -c ./Front_End/frontend.cpp $(FLAGS) -o obj/frontend.o

translator.o: ./Translator/translator.cpp
	g++ -c ./Translator/translator.cpp $(FLAGS) -o obj/translator.o

lexer.o: ./Front_End/lexer.cpp
	g++ -c ./Front_End/lexer.cpp $(FLAGS) -o obj/lexer.o

back_end.o: ./Back_end/back_end.cpp
	g++ -c ./Back_end/back_end.cpp $(FLAGS) -o obj/back_end.o

.PHONY: clean

clean:
	rm *.o list