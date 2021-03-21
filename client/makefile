client := bin/client
CC := g++
CFLAGS := -Wall

all: $(client)

$(client): build/main.o build/client.o 
	$(CC) $(CFLAGS) build/main.o build/client.o -o $(client)

build/main.o: src/main.cpp src/client.hpp
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o 

build/client.o: src/client.cpp src/client.hpp
	$(CC) $(CFLAGS) -c src/client.cpp -o build/client.o 

dir:
	@mkdir build
	@mkdir bin
		
clean:
	@rm build/*.o
	@rm bin/*