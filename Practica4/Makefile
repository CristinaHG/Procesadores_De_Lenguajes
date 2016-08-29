.SUFFIXES:

generated/prog: generated/main.o generated/y.tab.o
	gcc -o generated/prog generated/main.o generated/y.tab.o
generated/y.tab.o: generated/y.tab.c
	gcc -c generated/y.tab.c -o generated/y.tab.o
generated/main.o: main.c
	gcc -c main.c -o generated/main.o
generated/y.tab.c: generated/lex.yy.c yacc.y
	yacc -v yacc.y -o generated/y.tab.c
generated/lex.yy.c: tokensLex.l
	flex -o generated/lex.yy.c -ll tokensLex.l
clean:
	rm -R -f generated/*
	rm -f *~
all:
	make clean
	make generated/prueba
