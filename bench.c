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
static const char *lines[] = {
	"6/10/2026 19:47:15.9722  COMBAT_LOG_VERSION,22,ADVANCED_LOG_ENABLED,1,BUILD_VERSION,12.0.5,PROJECT_ID,1",
	"6/10/2026 20:00:23.8292  MAP_CHANGE,2534,\"March on Quel'Danas\",11152.500000,10842.500000,-4380.000000,-4845.000000",
	"6/10/2026 19:00:44.2942  ZONE_CHANGE,2939,\"Harandar\",16",
	"8/9/2026 10:27:19.8382  ENCOUNTER_START,3306,\"Chimaerus the Undreamt God\",16,20,2939",
	"8/9/2026 10:29:15.1602  ENCOUNTER_END,3306,\"Chimaerus the Undreamt God\",16,20,0,15683",
	"9/20/2026 01:27:19.9012  CHALLENGE_MODE_START,\"Ruby Life Pools\",2521,399,12,[10,9,147]",
	"9/20/2026 01:49:00.7512  CHALLENGE_MODE_END,2521,1,12,1337154,372.652802,2774.157471",
	"9/16/2026 20:17:29.4192  SPELL_DAMAGE,Player-1305-0CBD659E,\"Ferskentærte-Kazzak-EU\",0x514,0x80000000,Creature-0-1469-3004-19086-261874-0001AADD30,\"Blightscale Rawling\",0xa48,0x80000000,200685,\"Blade Dance\",0x1,Creature-0-1469-3004-19086-261874-0001AADD30,Vehicle-0-1469-3004-19086-257758-00002ADC87,1927350,8175069,0,0,1470,0,0,0,3,0,100,0,1533.06,-3.76,2610,4.9320,91,7934,10795,-1,1,0,0,0,nil,nil,nil,AOE",
};

int main() {
	struct timespec start, end;
	item items[sizeof(lines) / sizeof(lines[0])];
	for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
		items[i].len = strlen(lines[i]);
		items[i].str = lines[i];
	}
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	uint64_t bytes = 0;
	wl_event e;
	unsigned long int ts;
	for (unsigned long long i = 0; i < ITRS; i++) {
		const item* t = &items[i % (sizeof(lines) / sizeof(lines[0]))];
		wl_parse(&ts, &e, t->str);
		bytes += t->len;
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	
	double sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	
	double gbps = bytes / sec / 1e9;
	double lps = ITRS / sec / 1000;
	
	printf("Time: %.6f s\n", sec);
	printf("Throughput: %.3f GB/s\n", gbps);
	printf("Lines per second: %.3f k/s\n", lps);
	
	return 0;
}
