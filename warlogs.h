#ifndef WARLOGS_H
#define WARLOGS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum wl_return_code {
    wl_ok,
    wl_malformed_event_fields,
    wl_malformed_event_layout,
    wl_malformed_time_stamp
} wl_return_code;

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

typedef struct wl_event_undefined {
} wl_event_undefined;

typedef struct wl_event_version {
    unsigned int log;
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
    unsigned int project;
    bool         advanced;
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
    unsigned int time;
    char         name[128];
    bool         success;
} wl_event_encounter_end;

typedef struct wl_event_map_change {
    unsigned int id;
    char         name[128];
} wl_event_map_change;

typedef struct wl_event_zone_change {
    unsigned int instance;
    unsigned int difficulty;
    char         name[128];
} wl_event_zone_change;

typedef struct wl_event_unit_died {
    char id[128];
    char name[128];
} wl_event_unit_died;

typedef struct wl_event {
    wl_event_kind kind;
    union {
        wl_event_undefined       undefined;
        wl_event_version         version;
        wl_event_encounter_start encounter_start;
        wl_event_encounter_end   encounter_end;
        wl_event_map_change      map_change;
        wl_event_zone_change     zone_change;
        wl_event_unit_died       unit_died;
    };
} wl_event;

static inline wl_return_code wl_parse_version(wl_event *event, const char* str) {
    const char *format = "%d,ADVANCED_LOG_ENABLED,%d,BUILD_VERSION,%d.%d.%d,PROJECT_ID,%d";
    wl_event_version *e = &event->version;
    return sscanf(str, format, &e->log, &e->advanced, &e->major, &e->minor, &e->patch, &e->project) == 6
           ? wl_ok : wl_malformed_event_fields;
}

static inline wl_return_code wl_parse_encounter_start(wl_event *event, const char* str) {
    const char *format = "%d,\"%[^\"]\",%d,%d,%d";
    wl_event_encounter_start *e = &event->encounter_start;
    return sscanf(str, format, &e->encounter, &e->name, &e->difficulty, &e->group_size, &e->instance) == 5
           ? wl_ok : wl_malformed_event_fields;
}

static inline wl_return_code wl_parse_encounter_end(wl_event *event, const char* str) {
    const char *format = "%d,\"%[^\"]\",%d,%d,%d,%d";
    wl_event_encounter_end *e = &event->encounter_end;
    return sscanf(str, format, &e->encounter, &e->name, &e->difficulty, &e->group_size, &e->success, &e->time) == 6
           ? wl_ok : wl_malformed_event_fields;
}

static inline wl_return_code wl_parse_map_change(wl_event *event, const char* str) {
    const char *format = "%d,\"%[a-zA-Z ]\"";
    wl_event_map_change *e = &event->map_change;
    return sscanf(str, format, &e->id, e->name) == 2
           ? wl_ok : wl_malformed_event_fields;
}

static inline wl_return_code wl_parse_zone_change(wl_event *event, const char* str) {
    const char *format = "%d,\"%[a-zA-Z0-9 ]\",%d";
    wl_event_zone_change *e = &event->zone_change;
    return sscanf(str, format, &e->instance, e->name, &e->difficulty) == 3
           ? wl_ok : wl_malformed_event_fields;
}

static inline wl_return_code wl_parse_unit_died(wl_event *event, const char* str) {
    const char *format = "%*[^,],%*[^,],%*[^,],%*[^,],%[^,],\"%[^\"]";
    wl_event_unit_died *e = &event->unit_died;
    return sscanf(str, format, e->id, e->name) == 2
           ? wl_ok : wl_malformed_event_fields;
}

static inline wl_event_kind wl_match(const char* str) {
    for (unsigned int i = 0; i < wl_event_max - 1; i++)
        if (strcmp(str, WL_EVENT_NAMES[i]) == 0)
            return (wl_event_kind) i;
    return wl_undefined;
}

static inline wl_return_code wl_parse_timestamp(time_t* timestamp, const char* str) {
    const char *format = "%d/%d/%d %d:%d:%d";
    unsigned int day, month, year, hour, min, sec;
    if (sscanf(str, format, &month, &day, &year, &hour, &min, &sec) != 6)
        return wl_malformed_time_stamp;
    *timestamp = (year  - 1970) * 31622400 +
                 (month - 1)    * 2678400 +
                 (day   - 1)    * 86400 +
                 (hour  - 0)    * 3600 +
                 (min   - 0)    * 60 +
                 (sec   - 0)    * 1;
    return wl_ok;
}

static inline wl_return_code wl_parse(time_t* timestamp, wl_event *event, const char* str) {
    const char *format = "%*s%*s%*[ ]%[^,],%[^\n]";
    char event_name[128];
    char event_fields[4096];
    wl_return_code rc;
    if (sscanf(str, format, event_name, event_fields) != 2)
        return wl_malformed_event_layout;
    if ((rc = wl_parse_timestamp(timestamp, str)) != wl_ok)
        return rc;
    event->kind = wl_match(event_name);
    switch (event->kind) {
        case wl_version:         return wl_parse_version(event, event_fields);
        case wl_encounter_start: return wl_parse_encounter_start(event, event_fields);
        case wl_encounter_end:   return wl_parse_encounter_end(event, event_fields);
        case wl_map_change:      return wl_parse_map_change(event, event_fields);
        case wl_zone_change:     return wl_parse_zone_change(event, event_fields);
        case wl_unit_died:       return wl_parse_unit_died(event, event_fields);
        default: return wl_ok;
        }
}
#endif
