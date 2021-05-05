# CC is varilable
CC = g++

all	: bin/mps
	@echo -n ""

bin/mps	: src/main.cpp
		$(CC) $< -o $@

bin/ps_checker : src/ps_checker.cpp
		$(CC) $< -o $@

clean:
		rm -rf *.o bin/*