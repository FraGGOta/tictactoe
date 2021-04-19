tictactoe := bin/tictactoe
CC := g++
CFLAGS := -Wall

all: $(tictactoe)

$(tictactoe): build/main.o build/tictactoe.o 
	$(CC) $(CFLAGS) build/main.o build/tictactoe.o -o $(tictactoe) -lpthread

build/main.o: src/main.cpp src/tictactoe.hpp
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o 

build/tictactoe.o: src/tictactoe.cpp src/tictactoe.hpp
	$(CC) $(CFLAGS) -c src/tictactoe.cpp -o build/tictactoe.o

dir:
	@mkdir build
	@mkdir bin
		
clean:
	@rm build/*.o
	@rm bin/*