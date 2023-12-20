clean:
	rm -rf bin && rm -f wgrep
wgrep:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/wgrep wgrep.c
test:
	rm -f wcat && gcc -Wall -Wextra -std=c2x -pedantic -g -o wgrep wgrep.c && ./test-wgrep.sh -c && rm -f wgrep

memcheck:
	valgrind --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/wgrep search_this test_a.in test_b.in
