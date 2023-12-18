clean:
	rm -rf bin && rm -f reverse
reverse:
	mkdir -p bin && gcc -Wall -Wextra -std=c2x -pedantic -g -o bin/reverse reverse.c
test:
	rm -f reverse && gcc -Wall -Wextra -std=c2x -pedantic -g -o reverse reverse.c && ./test-reverse.sh -c
