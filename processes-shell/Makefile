clean:
	rm -rf bin
wish:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/wish wish.c
run:
	./bin/wish
wish_memcheck:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/wish -O0 wish.c
memcheck:
	valgrind --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/wish test.wish