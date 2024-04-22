FLAGS = -Wall -pthread -g

all: counter approx_counter

counter:
	mkdir -p bin && gcc -o bin/counter counter.c $(FLAGS)

approx_counter:
	mkdir -p bin && gcc -o bin/approx_counter approx_counter.c $(FLAGS)

concurrent_ll:
	mkdir -p bin && gcc -o bin/concurrent_ll concurrent_ll.c $(FLAGS)
