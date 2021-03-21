CC := g++
CFLAGS := -Wall

bin/prog: build/main.o build/TicTacToe.o 
	$(CC) $(CFLAGS) build/main.o build/TicTacToe.o -o bin/prog 

build/main.o: src/main.cpp src/TicTacToe.h
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o 

build/TicTacToe.o: src/TicTacToe.cpp src/TicTacToe.h
	$(CC) $(CFLAGS) -c src/TicTacToe.cpp -o build/TicTacToe.o

dir:
	@mkdir build
	@mkdir bin
		
clean:
	@rm build/*.o
	@rm bin/*