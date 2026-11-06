#include "warlogs.h"

#include <string.h>
#include <stdio.h>

wl_kind wl_parse(unsigned long int* ts, wl_event* e, const char* s) {
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

	if (sscanf(&s[offset], "MAP_CHANGE,%d,\"%[^\"]\",%f,%f,%f,%f", &e->mapchange.map, e->mapchange.name, &e->mapchange.x0, &e->mapchange.x1, &e->mapchange.y0, &e->mapchange.y1) == 6)
		return wl_kind_mapchange;

	if (sscanf(&s[offset], "ZONE_CHANGE,%d,\"%[^\"]\",%d", &e->zonechange.instance, e->zonechange.name, &e->zonechange.difficulty) == 3)
		return wl_kind_zonechange;

	if (sscanf(&s[offset], "ENCOUNTER_START,%d,\"%[^\"]\",%d,%d,%d", &e->encounterstart.encounter, e->encounterstart.name, &e->encounterstart.difficulty, &e->encounterstart.groupsize, &e->encounterstart.instance) == 5)
		return wl_kind_encounterstart;

	if (sscanf(&s[offset], "ENCOUNTER_END,%d,\"%[^\"]\",%d,%d,%d,%d", &e->encounterend.encounter, e->encounterend.name, &e->encounterend.difficulty, &e->encounterend.groupsize, &e->encounterend.success, &e->encounterend.duration) == 6)
		return wl_kind_encounterend;

	return wl_kind_invalid;
}
