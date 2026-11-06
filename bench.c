#include "warlogs.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct item {
	size_t      len;
	const char* str;
} item;

#define ITRS 999999
#define LINE_COUNT 5
static const char *lines[LINE_COUNT] = {
	"6/10/2026 19:47:15.9722  COMBAT_LOG_VERSION,22,ADVANCED_LOG_ENABLED,1,BUILD_VERSION,12.0.5,PROJECT_ID,1",
	"6/10/2026 20:00:23.8292  MAP_CHANGE,2534,\"March on Quel'Danas\",11152.500000,10842.500000,-4380.000000,-4845.000000",
	"6/10/2026 19:00:44.2942  ZONE_CHANGE,2939,\"Harandar\",16",
	"8/9/2026 10:27:19.8382  ENCOUNTER_START,3306,\"Chimaerus the Undreamt God\",16,20,2939",
	"8/9/2026 10:29:15.1602  ENCOUNTER_END,3306,\"Chimaerus the Undreamt God\",16,20,0,15683",
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
	unsigned long int ts;
	for (unsigned long long i = 0; i < ITRS; i++) {
		const item* t = &items[i % LINE_COUNT];
		wl_parse(&ts, &e, t->str);
		bytes += t->len;
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	
	double sec =
	    (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	
	double gbps = bytes / sec / 1e9;
	double lps = ITRS / sec / 1000;
	
	printf("Time: %.6f s\n", sec);
	printf("Throughput: %.3f GB/s\n", gbps);
	printf("Lines per second: %.3f k/s\n", lps);
	
	return 0;
}
