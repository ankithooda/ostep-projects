FLAGS = -Wall -pthread -g

all: counter approx_counter

counter:
	mkdir -p bin && gcc -o bin/counter counter.c $(FLAGS)

approx_counter:
	mkdir -p bin && gcc -o bin/approx_counter approx_counter.c $(FLAGS)

concurrent_ll:
	mkdir -p bin && gcc -o bin/concurrent_ll concurrent_ll.c $(FLAGS)

concurrent_q:
	mkdir -p bin && gcc -o bin/concurrent_q concurrent_q.c $(FLAGS)

concurrent_q_better:
	mkdir -p bin && gcc -o bin/concurrent_q_better concurrent_q_better.c $(FLAGS)

