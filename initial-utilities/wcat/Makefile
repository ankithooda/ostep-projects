clean:
	rm -rf bin && rm -f wcat
wcat:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/wcat wcat.c
test:
	rm -f wcat && gcc -Wall -Wextra -std=c2x -pedantic -g -o wcat wcat.c && ./test-wcat.sh -c && rm -f wcat

memcheck:
	valgrind --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/wcat test_in
