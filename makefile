.PHONY: bench fuzz test

bench:
	gcc -O3 -g -march=native bench.c
	./a.out
	rm a.out

fuzz:
	clang -g -O1 -fsanitize=fuzzer,address,undefined fuzz.c
	./a.out -dict=fuzz_dict
	rm a.out

test:
