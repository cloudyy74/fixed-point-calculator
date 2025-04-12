CXXFLAGS = -O2 -pedantic -Wall -Wextra -std=c++20 -Iinclude

all: obj main

obj:
	mkdir -p obj

test: obj obj/fixed_point.o obj/test.o include/fixed_point.hpp
	clang++	$(CXXFLAGS) obj/fixed_point.o obj/test.o -o test

main: obj/fixed_point.o obj/main.o include/fixed_point.hpp
	clang++	$(CXXFLAGS) obj/fixed_point.o obj/main.o -o main

obj/fixed_point.o: src/fixed_point.cpp include/fixed_point.hpp
	clang++ -c $(CXXFLAGS) src/fixed_point.cpp -o obj/fixed_point.o

obj/main.o: main.cpp include/fixed_point.hpp
	clang++ -c $(CXXFLAGS) main.cpp -o obj/main.o


clean:
	rm -rf obj main

.PHONY: clean all
