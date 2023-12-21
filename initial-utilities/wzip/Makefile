clean:
	rm -rf bin && rm -f wzip
wzip:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/wzip wzip.c
test:
	rm -f wcat && gcc -Wall -Wextra -std=c2x -pedantic -g -o wzip wzip.c && ./test-wzip.sh -c && rm -f wzip
memcheck:
	valgrind --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/wzip
