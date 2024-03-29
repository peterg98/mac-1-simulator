CC=g++

build: scanner.cc lex.yy.c
	mkdir -p out
	$(CC) scanner.cc lex.yy.c -o translator -std=c++17
	./translator
	$(CC) simulator.cc -o simulator

simulate: simulator.cc
	./simulator out/add1 file
	./simulator out/add2 file
	./simulator out/add3 file
	
lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm -rf lex.yy.c translator simulator out/*