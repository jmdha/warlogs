#ifndef WARLOGS_H
#define WARLOGS_H

typedef enum {
	wl_kind_invalid,
	wl_kind_version,
	wl_kind_mapchange,
	wl_kind_zonechange,
	wl_kind_encounterstart,
	wl_kind_encounterend,
} wl_kind;

typedef struct { int log, advanced, major, minor, patch, project; } wl_version;
typedef struct { int map; float x0, x1, y0, y1; char name[64]; } wl_mapchange;
typedef struct { int instance, difficulty; char name[64]; } wl_zonechange;
typedef struct { int encounter, difficulty, groupsize, instance; char name[64]; } wl_encounterstart;
typedef struct { int encounter, difficulty, groupsize, success, duration; char name[64]; } wl_encounterend;

typedef union {
	wl_version version;
	wl_mapchange mapchange;
	wl_zonechange zonechange;
	wl_encounterstart encounterstart;
	wl_encounterend encounterend;
} wl_event;

wl_kind wl_parse(unsigned long int* ts, wl_event* e, const char* s);

#endif
