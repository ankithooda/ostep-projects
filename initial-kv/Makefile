clean:
	rm -rf bin && rm -f kv
kv:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/kv kv.c
test:
	rm -f wcat && gcc -Wall -Wextra -std=c2x -pedantic -g -o kv kv.c && ./test-kv.sh -c && rm -f kv
memcheck:
	valgrind --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/kv
