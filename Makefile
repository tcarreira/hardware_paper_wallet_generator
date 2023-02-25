all: clean test
	@echo "Nothing to do here"

test:
	@gcc -std=c99 -Wall tests.c -o test.out
	@./test.out
	@echo "Tests successfully passed"

clean:
	rm -rf *.out
