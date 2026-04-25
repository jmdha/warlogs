#ifndef WARLOGS_H
#define WARLOGS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum wl_event_kind {
	wl_version,
	wl_challenge_mode_start,
	wl_challenge_mode_end,
	wl_encounter_start,
	wl_encounter_end,
	wl_combatant_info,
	wl_map_change,
	wl_zone_change,
	wl_unit_died,
	wl_party_kill,
	wl_spell_cast_success,
	wl_spell_summon,
	wl_spell_instakill,
	wl_spell_energize,
	wl_spell_heal,
	wl_spell_damage,
	wl_spell_periodic_damage,
	wl_spell_periodic_heal,
	wl_spell_aura_applied,
	wl_spell_aura_refresh,
	wl_spell_aura_removed,
	wl_undefined,
	wl_event_max,
} wl_event_kind;

static const char* WL_EVENT_NAMES[] = {
	"COMBAT_LOG_VERSION",
	"CHALLENGE_MODE_START",
	"CHALLENGE_MODE_END",
	"ENCOUNTER_START",
	"ENCOUNTER_END",
	"COMBATANT_INFO",
	"MAP_CHANGE",
	"ZONE_CHANGE",
	"UNIT_DIED",
	"PARTY_KILL",
	"SPELL_CAST_SUCCESS",
	"SPELL_SUMMON",
	"SPELL_INSTAKILL",
	"SPELL_ENERGIZE",
	"SPELL_HEAL",
	"SPELL_DAMAGE",
	"SPELL_PERIODIC_DAMAGE",
	"SPELL_PERIODIC_HEAL",
	"SPELL_AURA_APPLIED",
	"SPELL_AURA_REFRESH",
	"SPELL_AURA_REMOVED",
	"UNDEFINED"
};

static const char* wl_error;
static const char* wl_error_ptr;

typedef struct wl_event_version {
	unsigned int log;
	unsigned int major;
	unsigned int minor;
	unsigned int patch;
	unsigned int project;
	unsigned int advanced;
} wl_event_version;

typedef struct wl_event_encounter_start {
	unsigned int encounter;
	unsigned int difficulty;
	unsigned int group_size;
	unsigned int instance;
	char         name[128];
} wl_event_encounter_start;

typedef struct wl_event_encounter_end {
	unsigned int encounter;
	unsigned int difficulty;
	unsigned int group_size;
	unsigned int duration;
	unsigned int success;
	char         name[128];
} wl_event_encounter_end;

typedef struct wl_event_map_change {
	unsigned int instance;
	char         name[128];
} wl_event_map_change;

typedef struct wl_event_zone_change {
	unsigned int instance;
	unsigned int difficulty;
	char         name[128];
} wl_event_zone_change;

typedef struct wl_event_unit_died {
	char guid[128];
	char name[128];
} wl_event_unit_died;

typedef struct wl_event_spell_damage {
	char source_guid[128];
	char source_name[128];
	char target_guid[128];
	char target_name[128];
	int  spell_id;
	char spell_name[128];
	int  damage_raw;
	int  damage;
} wl_event_spell_damage;

typedef struct wl_event {
	wl_event_kind kind;
	union {
		wl_event_version               version;
		wl_event_encounter_start       encounter_start;
		wl_event_encounter_end         encounter_end;
		wl_event_map_change            map_change;
		wl_event_zone_change           zone_change;
		wl_event_unit_died             unit_died;
		wl_event_spell_damage          spell_damage;
	};
} wl_event;

static inline int wl_is_sentinel(char c, char sentinel) {
	return c == sentinel || c == '\r' || c == '\n' || c == '\0';
}

static const char* wl_parse_str(char* out, const char* str, char sentinel) {
	bool qoute = *str == '"';
	if (qoute) str++;
	do {
		if (qoute && *str == '"') {
			str++;
			break;
		}
		if (!qoute && wl_is_sentinel(*str, sentinel))
			break;
		*out++ = *str++;
	} while (true);
	if (!*str) { wl_error = "unexpected eof"; wl_error_ptr = str; return NULL; }
	*out = '\0';
	return str + 1;
}

static const char* wl_parse_int(int* val, const char* str, char sentinel) {
	int neg = 0;
	int ret = 0;
	if (*str == '-') { neg = 1; str++; }
	while (!wl_is_sentinel(*str, sentinel)) {
		if (*str < '0' || *str > '9') {
			wl_error = "non-numeric character";
			wl_error_ptr = str;
			return NULL;
		}
		ret = ret * 10 + (*str++ - '0');
	}
	if (!*str) { wl_error = "unexpected eof"; wl_error_ptr = str; return NULL; }
	*val = neg ? -ret : ret;
	return str + 1;
}

static const char* wl_parse_skip(const char* str, char sentinel) {
	while (!wl_is_sentinel(*str, sentinel)) str++;
	if (!*str) { wl_error = "unexpected eof"; wl_error_ptr = str; return NULL; }
	return str + 1;
}

static const char* wl_parse_timestamp(int64_t* ts, const char* str) {
	int day, month, year, hour, min, sec, ns;
	if (!(str = wl_parse_int(&month, str, '/'))) return NULL;
	if (!(str = wl_parse_int(&day,   str, '/'))) return NULL;
	if (!(str = wl_parse_int(&year,  str, ' '))) return NULL;
	if (!(str = wl_parse_int(&hour,  str, ':'))) return NULL;
	if (!(str = wl_parse_int(&min,   str, ':'))) return NULL;
	if (!(str = wl_parse_int(&sec,   str, '.'))) return NULL;
	if (!(str = wl_parse_int(&ns,    str, ' '))) return NULL;
	*ts = (int64_t)(year  - 1970) * 31622400000LL +
	      (int64_t)(month - 1)    * 2678400000LL +
	      (int64_t)(day   - 1)    * 86400000LL +
	      (int64_t)(hour)         * 3600000LL +
	      (int64_t)(min)          * 60000LL +
	      (int64_t)(sec)          * 1000LL +
	      (int64_t)(ns);
	return str;
}

static const char* wl_parse_version(wl_event* e, const char* str) {
	if (!(str = wl_parse_int(&e->version.log,      str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                     str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->version.advanced, str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                     str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->version.major,    str, '.')))  return NULL;
	if (!(str = wl_parse_int(&e->version.minor,    str, '.')))  return NULL;
	if (!(str = wl_parse_int(&e->version.patch,    str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                     str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->version.project,  str, '\n'))) return NULL;
	return str;
}

static const char* wl_parse_encounter_start(wl_event* e, const char* str) {
	if (!(str = wl_parse_int(&e->encounter_start.encounter,  str, ',')))  return NULL;
	if (!(str = wl_parse_str(e->encounter_start.name,        str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->encounter_start.difficulty, str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->encounter_start.group_size, str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->encounter_start.instance,   str, ',')))  return NULL;
	return str;
}

static const char* wl_parse_encounter_end(wl_event* e, const char* str) {
	if (!(str = wl_parse_int(&e->encounter_end.encounter,  str, ',')))  return NULL;
	if (!(str = wl_parse_str(e->encounter_end.name,        str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->encounter_end.difficulty, str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->encounter_end.group_size, str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->encounter_end.success,    str, ',')))  return NULL;
	if (!(str = wl_parse_int(&e->encounter_end.duration,   str, ',')))  return NULL;
	return str;
}

static const char* wl_parse_map_change(wl_event* e, const char* str) {
	if (!(str = wl_parse_int(&e->map_change.instance, str, ',')))  return NULL;
	if (!(str = wl_parse_str(e->map_change.name,      str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, '\n'))) return NULL;
	return str;
}

static const char* wl_parse_zone_change(wl_event* e, const char* str) {
	if (!(str = wl_parse_int(&e->zone_change.instance,   str, ','))) return NULL;
	if (!(str = wl_parse_str(e->zone_change.name,        str, ','))) return NULL;
	if (!(str = wl_parse_int(&e->zone_change.difficulty, str, ','))) return NULL;
	return str;
}

static const char* wl_parse_unit_died(wl_event* e, const char* str) {
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_str(e->unit_died.guid,       str, ',')))  return NULL;
	if (!(str = wl_parse_str(e->unit_died.name,       str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	if (!(str = wl_parse_skip(                        str, ',')))  return NULL;
	return str;
}

static const char* wl_parse_spell_damage(wl_event* e, const char* str) {
	wl_event_spell_damage* E = &e->spell_damage;
	if (!(str = wl_parse_str(E->source_guid,  str, ',')))   return NULL;
	if (!(str = wl_parse_str(E->source_name,  str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_str(E->target_guid,  str, ',')))   return NULL;
	if (!(str = wl_parse_str(E->target_name,  str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_int(&E->spell_id,    str, ',')))   return NULL;
	if (!(str = wl_parse_str(E->spell_name,   str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_int(&E->damage,      str, ',')))   return NULL;
	if (!(str = wl_parse_int(&E->damage_raw,  str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, ',')))   return NULL;
	if (!(str = wl_parse_skip(                str, '\n')))  return NULL;
	return str;
}

static inline wl_event_kind wl_match(const char* str) {
	for (unsigned int i = 0; i < wl_event_max - 1; i++)
		if (strcmp(str, WL_EVENT_NAMES[i]) == 0)
			return (wl_event_kind) i;
	return wl_undefined;
}

static const char* wl_parse_event(wl_event* e, const char* str) {
	char name[256];
	const char* ptr = str;
	if (!(ptr = wl_parse_str(name, ptr, ','))) return NULL;

	e->kind = wl_match(name);
	switch (e->kind) {
	case wl_version:               return wl_parse_version(e, ptr);
	case wl_encounter_start:       return wl_parse_encounter_start(e, ptr);
	case wl_encounter_end:         return wl_parse_encounter_end(e, ptr);
	case wl_map_change:            return wl_parse_map_change(e, ptr);
	case wl_zone_change:           return wl_parse_zone_change(e, ptr);
	case wl_unit_died:             return wl_parse_unit_died(e, ptr);
	case wl_spell_periodic_damage:
	case wl_spell_damage:          return wl_parse_spell_damage(e, ptr);
	default:
			  e->kind = wl_undefined;
			  return strchr(str, '\n');
			  wl_error = "unknown event";
			  wl_error_ptr = str;
			  return NULL;
	}
}

static const char* wl_parse(int64_t* ts, wl_event *e, const char* str) {
	wl_error     = NULL;
	wl_error_ptr = NULL;

	str = wl_parse_timestamp(ts, str);
	if (!str) return NULL;

	// timestamp is followed by 2 spaces, skip those
	while (*str == ' ') str++;

	str = wl_parse_event(e, str);
	if (!str) return NULL;

	// allows both \n and \0 terminated strings
	if (*str == '\n') str++;

	return str;
}

#endif
