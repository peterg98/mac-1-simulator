CC=g++

build: scanner.cc lex.yy.c
	$(CC) scanner.cc lex.yy.c -o translator
	./translator test1.mac
	$(CC) simulator.cc -o simulator
	
lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm -rf lex.yy.c translator simulator machine-code