# SAMIP JASANI 2015A7PS0127P

run : linkall
	./stage1exe ./testcases/testcase5.txt ./parsetreeoutfile.txt

linkall: driver
	gcc lexer.o _Tree.o ast.o _Trie.o _Stack.o parser.o driver.o -o stage1exe

gdb : linkall
	gdb --args ./stage1exe ./testcases/testcase2.txt ./parsetreeoutfile.txt

driver: lexer parser ast driver.c
	gcc -c -g driver.c

lexer: lexer.h lexerDef.h lexer.c _Trie.c _Trie.h
	gcc -c -g _Trie.c
	gcc -c -g lexer.c

parser: lexer _Stack.c _Stack.h _Tree.c _Tree.h
	gcc -c -g _Stack.c
	gcc -c -g parser.c
	gcc -c -g _Tree.c

ast: 
	gcc -c -g ast.c


clean:
	rm *.o stage1exe parsetreeoutfile*