all:
	gcc -o test warlogs_test.c -lcriterion -lm
	./test
