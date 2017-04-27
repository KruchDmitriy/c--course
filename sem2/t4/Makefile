CPP_FLAGS=-Wall -Werror
all: bin bin/main

bin/main: bin/a.o bin/main.o
	g++ $(CPP_FLAGS) bin/main.o bin/a.o -o bin/main

bin/main.o: src/main.cpp
	g++ $(CPP_FLAGS) -c src/main.cpp -o bin/main.o

bin/a.o: src/a.cpp src/a.hpp bin/add.o
	g++ $(CPP_FLAGS) -c src/a.cpp bin/add.o -o bin/a.o

bin/add.o: src/add.cpp src/add.hpp
	g++ $(CPP_FLAGS) -c src/add.cpp -o bin/add.o

bin:
	mkdir -p bin

clean:
	rm -f bin/*

.PHONY: clean all