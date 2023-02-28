all: clean test
	@echo "Nothing to do here"

test:
	@gcc -std=c99 -Wall *.c -o test.out
	@./test.out
	@echo "Tests successfully passed"

testing_mac:
	@echo "Starting continuous testing. CTRL+C to stop."
	@sh -c "date ; ${MAKE} test " || true
	@fswatch -o *.c *.h | xargs -n1 -I{} sh -c "echo ''; date ; ${MAKE} test "

format:
	@clang-format -i *.c *.h

clean:
	rm -rf *.out
