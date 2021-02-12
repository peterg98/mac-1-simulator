CC=g++

build: scanner.cc lex.yy.c
	$(CC) scanner.cc lex.yy.c -o translator
	
lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm -rf lex.yy.c translator