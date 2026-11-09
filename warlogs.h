#ifndef WARLOGS_H
#define WARLOGS_H

#include <string.h>
#include <stdio.h>

typedef enum {
	wl_kind_invalid,
	wl_kind_version,
	wl_kind_mapchange,
	wl_kind_zonechange,
	wl_kind_encounterstart,
	wl_kind_encounterend,
	wl_kind_challengemodestart,
	wl_kind_challengemodeend,
	wl_kind_spelldamage,
} wl_kind;

typedef struct { int log, advanced, major, minor, patch, project; } wl_version;
typedef struct { int map; float x0, x1, y0, y1; char name[64]; } wl_mapchange;
typedef struct { int instance, difficulty; char name[64]; } wl_zonechange;
typedef struct { int encounter, difficulty, groupsize, instance; char name[64]; } wl_encounterstart;
typedef struct { int encounter, difficulty, groupsize, success, duration; char name[64]; } wl_encounterend;
typedef struct { int instance, challenge, level; char zone[64]; } wl_challengemodestart;
typedef struct { int instance, success, level, duration; } wl_challengemodeend;
typedef struct { int spell, raw, amount, overkill; char name[64], sguid[64], sname[64], tguid[64], tname[64], owner[64]; } wl_spelldamage;

typedef union {
	wl_version version;
	wl_mapchange mapchange;
	wl_zonechange zonechange;
	wl_encounterstart encounterstart;
	wl_encounterend encounterend;
	wl_challengemodestart challengemodestart;
	wl_challengemodeend challengemodeend;
	wl_spelldamage spelldamage;
} wl_event;

static wl_kind wl_parse(unsigned long int* ts, wl_event* e, const char* s) {
	unsigned long int y, m, d, H, M, S, F;
	int offset;

	if (sscanf(s, "%ld/%ld/%ld %ld:%ld:%ld.%ld  %n", &m, &d, &y, &H, &M, &S, &F, &offset) != 7)
		return wl_kind_invalid;

	if (y < 2000 || y > 2040 || m < 1 || m > 12 || d < 1 || d > 31 || H > 23 || M > 59 || S > 59 || F > 9999)
		return wl_kind_invalid;

	*ts = (y - 1970) * 31622400000 +
	      (m - 1)    * 2678400000 +
	      (d - 1)    * 86400000 +
	      (H)        * 3600000 +
	      (M)        * 60000 +
	      (S)        * 1000 +
	      (F);

	if (sscanf(&s[offset], "COMBAT_LOG_VERSION,%d,ADVANCED_LOG_ENABLED,%d,BUILD_VERSION,%d.%d.%d,PROJECT_ID,%d", &e->version.log, &e->version.advanced, &e->version.major, &e->version.minor, &e->version.patch, &e->version.project) == 6)
		return wl_kind_version;

	if (sscanf(&s[offset], "MAP_CHANGE,%d,\"%63[^\"]\",%f,%f,%f,%f", &e->mapchange.map, e->mapchange.name, &e->mapchange.x0, &e->mapchange.x1, &e->mapchange.y0, &e->mapchange.y1) == 6)
		return wl_kind_mapchange;

	if (sscanf(&s[offset], "ZONE_CHANGE,%d,\"%63[^\"]\",%d", &e->zonechange.instance, e->zonechange.name, &e->zonechange.difficulty) == 3)
		return wl_kind_zonechange;

	if (sscanf(&s[offset], "ENCOUNTER_START,%d,\"%63[^\"]\",%d,%d,%d", &e->encounterstart.encounter, e->encounterstart.name, &e->encounterstart.difficulty, &e->encounterstart.groupsize, &e->encounterstart.instance) == 5)
		return wl_kind_encounterstart;

	if (sscanf(&s[offset], "ENCOUNTER_END,%d,\"%63[^\"]\",%d,%d,%d,%d", &e->encounterend.encounter, e->encounterend.name, &e->encounterend.difficulty, &e->encounterend.groupsize, &e->encounterend.success, &e->encounterend.duration) == 6)
		return wl_kind_encounterend;

	if (sscanf(&s[offset], "CHALLENGE_MODE_START,\"%63[^\"]\",%d,%d,%d", e->challengemodestart.zone, &e->challengemodestart.instance, &e->challengemodestart.challenge, &e->challengemodestart.level) == 4)
		return wl_kind_challengemodestart;

	if (sscanf(&s[offset], "CHALLENGE_MODE_END,%d,%d,%d,%d", &e->challengemodeend.instance, &e->challengemodeend.success, &e->challengemodeend.level, &e->challengemodeend.duration) == 4)
		return wl_kind_challengemodeend;

	if (sscanf(&s[offset], "SPELL_DAMAGE,%63[^,],\"%63[^\"]\",%*x,%*x,%63[^,],\"%63[^\"]\",%*x,%*x,%d,\"%63[^\"]\",%*x,%*[^,],%63[^,],%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*f,%*f,%*d,%*f,%*d,%d,%d,%d", e->spelldamage.sguid, e->spelldamage.sname, e->spelldamage.tguid, e->spelldamage.tname, &e->spelldamage.spell, e->spelldamage.name, e->spelldamage.owner, &e->spelldamage.amount, &e->spelldamage.raw, &e->spelldamage.overkill) == 10)
		return wl_kind_spelldamage;

	return wl_kind_invalid;
}

#endif
