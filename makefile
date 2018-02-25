run : linkall
	./exe ./testcases/testcase1.txt

linkall: driver
	gcc lexer.o _Tree.o _Trie.o _Stack.o parser.o driver.o -o exe

gdb : linkall
	gdb exe

driver: lexer parser driver.c
	gcc -c -g driver.c

lexer: lexer.h lexerDef.h lexer.c _Trie.c _Trie.h
	gcc -c -g _Trie.c
	gcc -c -g lexer.c

parser: lexer _Stack.c _Stack.h _Tree.c _Tree.h
	gcc -c -g _Stack.c
	gcc -c -g parser.c
	gcc -c -g _Tree.c

clean:
	rm *.o exe output* a.out