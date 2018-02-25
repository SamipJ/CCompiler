run : linkall
	./exe ./testcases/testcase1.txt

linkall: driver
	gcc lexer.o Trie.o parser.o driver.o -o exe

gdb : linkall
	gdb exe

driver: lexer parser driver.c
	gcc -c -g driver.c

lexer: lexer.h lexerDef.h lexer.c Trie.c
	gcc -c -g Trie.c
	gcc -c -g lexer.c

parser: lexer
	gcc -c -g parser.c

clean:
	rm *.o exe output