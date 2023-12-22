clean:
	rm -rf bin && rm -f wunzip
wunzip:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/wunzip wunzip.c
test:
	rm -f wcat && gcc -Wall -Wextra -std=c2x -pedantic -g -o wunzip wunzip.c && ./test-wunzip.sh -c && rm -f wunzip
memcheck:
	valgrind --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/wunzip tests/6.in tests/6.in
