tictaktoe := bin/tictaktoe
CC := g++
CFLAGS := -Wall

all: $(tictaktoe)

$(tictaktoe): build/main.o build/tictaktoe.o 
	$(CC) $(CFLAGS) build/main.o build/tictaktoe.o -o $(tictaktoe) -lpthread

build/main.o: src/main.cpp src/tictaktoe.hpp
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o 

build/tictaktoe.o: src/tictaktoe.cpp src/tictaktoe.hpp
	$(CC) $(CFLAGS) -c src/tictaktoe.cpp -o build/tictaktoe.o

dir:
	@mkdir build
	@mkdir bin
		
clean:
	@rm build/*.o
	@rm bin/*