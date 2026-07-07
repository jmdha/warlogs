#include "warlogs.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct item {
	size_t      len;
	const char* str;
} item;

#define LINE_COUNT 4
static const char *lines[LINE_COUNT] = {
	"1/2/2000 16:26:02.0002",
	"12/12/2001 14:02:00.1234",
	"3/25/2002 02:13:45.4596",
	"5/16/2003 23:25:59.4321"
};

int main() {
	struct timespec start, end;
	item items[LINE_COUNT];
	for (size_t i = 0; i < LINE_COUNT; i++) {
		items[i].len = strlen(lines[i]);
		items[i].str = lines[i];
	}
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	uint64_t bytes = 0;
	wl_event e;
	int64_t   ts;
	for (unsigned long long i = 0; i < 99999999; i++) {
		const item* t = &items[i % LINE_COUNT];
		wl_parse(&ts, &e, t->str, t->len);
		bytes += t->len;
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	
	double sec =
	    (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	
	double gbps = bytes / sec / 1e9;
	
	printf("Time: %.6f s\n", sec);
	printf("Throughput: %.3f GB/s\n", gbps);
	
	return 0;
}
