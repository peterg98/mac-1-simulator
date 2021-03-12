CC=g++

build: scanner.cc lex.yy.c
	$(CC) scanner.cc lex.yy.c -o translator -std=c++17
	./translator
	$(CC) simulator.cc -o simulator

simulate: simulator.cc
	./simulator out/test2
	
lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm -rf lex.yy.c translator simulator out/*