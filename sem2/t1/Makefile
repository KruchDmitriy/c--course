CPP_FLAGS=-Wall -Werror
all: bin main

main: bin/add.o bin/main.o
	g++ $(CPP_FLAGS) bin/main.o bin/add.o -o bin/main

/bin/main.o: src/main.cpp
	g++ $(CPP_FLAGS) -c src/main.cpp -o bin/main.o

/bin/add.o: src/add.cpp
	g++ $(CPP_FLAGS) -c src/add.cpp -o bin/add.o

bin:
	mkdir -p bin

clean:
	rm -f bin/*

.PHONY: clean all