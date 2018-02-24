run : linkall
	./exe

linkall: driver
	gcc lexer.o Trie.o driver.o -o exe

gdb : linkall
	gdb exe

driver: lexer driver.c
	gcc -c -g driver.c

lexer: lexer.h lexerDef.h lexer.c Trie.c
	gcc -c -g Trie.c
	gcc -c -g lexer.c

clean:
	rm *.o exe output