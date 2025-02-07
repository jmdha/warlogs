#ifndef WARLOGS_H
#define WARLOGS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum wl_return_code {
    wl_ok,
    wl_malformed_time_stamp,
    wl_malformed_event_layout
} wl_return_code;

typedef enum wl_event_kind {
    wl_version,
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
    char* str;
} wl_event_undefined;

typedef struct wl_event_version {
    unsigned int log;
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
    bool         advanced;
} wl_event_version;

typedef struct wl_event_map_change {
    unsigned int id;
    char*        name;
} wl_event_map_change;

typedef struct wl_event_zone_change {
    unsigned int instance;
    unsigned int difficulty;
    char*        name;
} wl_event_zone_change;

typedef struct wl_event {
    wl_event_kind kind;
    union {
        wl_event_undefined   undefined;
        wl_event_version     version;
        wl_event_map_change  map_change;
        wl_event_zone_change zone_change;
    };
} wl_event;

static inline wl_event_kind wl_match(const char* str) {
    for (unsigned int i = 0; i < wl_event_max - 1; i++)
        if (strcmp(str, WL_EVENT_NAMES[i]) == 0)
            return i;
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
    const char *format = "%*s%*s %[^,]%s^,%s";
    char event_name[128];
    char event_fields[128];
    wl_return_code rc;
    if (sscanf(str, format, event_name, event_fields) != 2)
        return wl_malformed_event_layout;
    if ((rc = wl_parse_timestamp(timestamp, str)) != wl_ok)
        return rc;
    event->kind = wl_match(event_name);
    return wl_ok;
}
#endif
