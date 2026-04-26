#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "warlogs.h"

static const char* schema =
	"create table if not exists wl_version ("
	"	id        integer primary key,"
	"	timestamp integer not null,"
	"	log       integer not null,"
	"	major     integer not null,"
	"	minor     integer not null,"
	"	patch     integer not null,"
	"	project   integer not null,"
	"	advanced  integer not null"
	");"
	"create table if not exists wl_encounter_start ("
	"	id         integer primary key,"
	"	timestamp  integer not null,"
	"	encounter  integer not null,"
	"	difficulty integer not null,"
	"	group_size integer not null,"
	"	instance   integer not null,"
	"	name       text not null"
	");"
	"create table if not exists wl_encounter_end ("
	"	id         integer primary key,"
	"	timestamp  integer not null,"
	"	encounter  integer not null,"
	"	difficulty integer not null,"
	"	group_size integer not null,"
	"	duration   integer not null,"
	"	success    integer not null,"
	"	name       text not null"
	");"
	"create table if not exists wl_map_change ("
	"	id         integer primary key,"
	"	timestamp  integer not null,"
	"	instance   integer not null,"
	"	name       text not null"
	");"
	"create table if not exists wl_zone_change ("
	"	id         integer primary key,"
	"	timestamp  integer not null,"
	"	instance   integer not null,"
	"	difficulty integer not null,"
	"	name       text not null"
	");"
	"create table if not exists wl_unit_died ("
	"	id         integer primary key,"
	"	timestamp  integer not null,"
	"	guid       text not null,"
	"	name       text not null"
	");"
	"create table if not exists wl_spell_damage ("
	"	id          integer primary key,"
	"	timestamp   integer not null,"
	"	source_guid text not null,"
	"	source_name text not null,"
	"	target_guid text not null,"
	"	target_name text not null,"
	"	spell_id    integer not null,"
	"	spell_name  text not null,"
	"	damage_raw  integer not null,"
	"	damage      integer not null"
	");"
	"";

static sqlite3*      db;
static sqlite3_stmt* st_version;
static sqlite3_stmt* st_challenge_mode_start;
static sqlite3_stmt* st_challenge_mode_end;
static sqlite3_stmt* st_encounter_start;
static sqlite3_stmt* st_encounter_end;
static sqlite3_stmt* st_combatant_info;
static sqlite3_stmt* st_map_change;
static sqlite3_stmt* st_zone_change;
static sqlite3_stmt* st_unit_died;
static sqlite3_stmt* st_party_kill;
static sqlite3_stmt* st_spell_cast_success;
static sqlite3_stmt* st_spell_summon;
static sqlite3_stmt* st_spell_instakill;
static sqlite3_stmt* st_spell_energize;
static sqlite3_stmt* st_spell_heal;
static sqlite3_stmt* st_spell_damage;
static sqlite3_stmt* st_spell_periodic_damage;
static sqlite3_stmt* st_spell_periodic_heal;
static sqlite3_stmt* st_spell_aura_applied;
static sqlite3_stmt* st_spell_aura_refresh;
static sqlite3_stmt* st_spell_aura_removed;

#define BIND_INT(db, stmt, idx, val)                                 \
    do {                                                             \
        int _rc = sqlite3_bind_int((stmt), (idx), (val));            \
        if (_rc != SQLITE_OK) {                                      \
            fprintf(stderr, "bind_int error [%d]: %s\n",             \
                    _rc, sqlite3_errmsg(db));                        \
            return _rc;                                              \
        }                                                            \
    } while (0)

#define BIND_INT64(db, stmt, idx, val)                               \
    do {                                                             \
        int _rc = sqlite3_bind_int64((stmt), (idx), (val));          \
        if (_rc != SQLITE_OK) {                                      \
            fprintf(stderr, "bind_int64 error [%d]: %s\n",           \
                    _rc, sqlite3_errmsg(db));                        \
            return _rc;                                              \
        }                                                            \
    } while (0)

#define BIND_TEXT(db, stmt, idx, val)                                \
    do {                                                             \
        int _rc = sqlite3_bind_text((stmt), (idx), (val), -1, NULL); \
        if (_rc != SQLITE_OK) {                                      \
            fprintf(stderr, "bind_int64 error [%d]: %s\n",           \
                    _rc, sqlite3_errmsg(db));                        \
            return _rc;                                              \
        }                                                            \
    } while (0)

int insert_version(int64_t ts, const wl_event* e) {
	sqlite3_reset(st_version);
	BIND_INT64(db, st_version, 1, ts);
	BIND_INT(db,   st_version, 2, e->version.log);
	BIND_INT(db,   st_version, 3, e->version.major);
	BIND_INT(db,   st_version, 4, e->version.minor);
	BIND_INT(db,   st_version, 5, e->version.patch);
	BIND_INT(db,   st_version, 6, e->version.project);
	BIND_INT(db,   st_version, 7, e->version.advanced);
	return sqlite3_step(st_version);
}

int insert_encounter_start(int64_t ts, const wl_event* e) {
	sqlite3_reset(st_encounter_start);
	BIND_INT64(db, st_encounter_start, 1, ts);
	BIND_INT(db,   st_encounter_start, 2, e->encounter_start.encounter);
	BIND_INT(db,   st_encounter_start, 3, e->encounter_start.difficulty);
	BIND_INT(db,   st_encounter_start, 4, e->encounter_start.group_size);
	BIND_INT(db,   st_encounter_start, 5, e->encounter_start.instance);
	BIND_TEXT(db,  st_encounter_start, 6, e->encounter_start.name);
	return sqlite3_step(st_encounter_start);
}

int insert_encounter_end(int64_t ts, const wl_event* e) {
	sqlite3_reset(st_encounter_end);
	BIND_INT64(db, st_encounter_end, 1, ts);
	BIND_INT(db,   st_encounter_end, 2, e->encounter_end.encounter);
	BIND_INT(db,   st_encounter_end, 3, e->encounter_end.difficulty);
	BIND_INT(db,   st_encounter_end, 4, e->encounter_end.group_size);
	BIND_INT(db,   st_encounter_end, 5, e->encounter_end.duration);
	BIND_INT(db,   st_encounter_end, 6, e->encounter_end.success);
	BIND_TEXT(db,  st_encounter_end, 7, e->encounter_end.name);
	return sqlite3_step(st_encounter_end);
}

int insert_map_change(int64_t ts, const wl_event* e) {
	sqlite3_reset(st_map_change);
	BIND_INT64(db, st_map_change, 1, ts);
	BIND_INT(db,   st_map_change, 2, e->map_change.instance);
	BIND_TEXT(db,  st_map_change, 3, e->map_change.name);
	return sqlite3_step(st_map_change);
}

int insert_zone_change(int64_t ts, const wl_event* e) {
	sqlite3_reset(st_zone_change);
	BIND_INT64(db, st_zone_change, 1, ts);
	BIND_INT(db,   st_zone_change, 2, e->zone_change.instance);
	BIND_INT(db,   st_zone_change, 3, e->zone_change.difficulty);
	BIND_TEXT(db,  st_zone_change, 4, e->zone_change.name);
	return sqlite3_step(st_zone_change);
}

int insert_unit_died(int64_t ts, const wl_event* e) {
	sqlite3_reset(st_unit_died);
	BIND_INT64(db, st_unit_died, 1, ts);
	BIND_TEXT(db,  st_unit_died, 2, e->unit_died.guid);
	BIND_TEXT(db,  st_unit_died, 3, e->unit_died.name);
	return sqlite3_step(st_unit_died);
}

int insert_spell_damage(int64_t ts, const wl_event* e) {
	sqlite3_reset(st_spell_damage);
	BIND_INT64(db, st_spell_damage, 1, ts);
	BIND_TEXT(db,  st_spell_damage, 2, e->spell_damage.source_guid);
	BIND_TEXT(db,  st_spell_damage, 3, e->spell_damage.source_name);
	BIND_TEXT(db,  st_spell_damage, 4, e->spell_damage.target_guid);
	BIND_TEXT(db,  st_spell_damage, 5, e->spell_damage.target_name);
	BIND_INT(db,   st_spell_damage, 6, e->spell_damage.spell_id);
	BIND_TEXT(db,  st_spell_damage, 7, e->spell_damage.spell_name);
	BIND_INT(db,   st_spell_damage, 8, e->spell_damage.damage_raw);
	BIND_INT(db,   st_spell_damage, 9, e->spell_damage.damage);
	return sqlite3_step(st_spell_damage);
}

int insert(int64_t ts, const wl_event* e) {
	switch (e->kind) {
		case wl_version:         return insert_version(ts, e);
		case wl_encounter_start: return insert_encounter_start(ts, e);
		case wl_encounter_end:   return insert_encounter_end(ts, e);
		case wl_map_change:      return insert_map_change(ts, e);
		case wl_zone_change:     return insert_zone_change(ts, e);
		case wl_unit_died:       return insert_unit_died(ts, e);
		case wl_spell_periodic_damage:
		case wl_spell_damage:    return insert_spell_damage(ts, e);
		default:
	}
	return SQLITE_OK;
}

int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "not enough arguments");
		return 1;
	}

	const char* in  = argv[1];
	const char* out = argv[2];

	const int fd = open(in, O_RDONLY);
        if (fd == -1) {
                perror(in);
		return 1;
        }

	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		perror(in);
		close(fd);
		return 1;
	}

	char* buf = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) {
		perror(in);
		close(fd);
		return 1;
	}
	madvise(buf, sb.st_size, MADV_SEQUENTIAL);

	if (sqlite3_open(out, &db) != SQLITE_OK) {
		fprintf(stderr, "failed to open output file\n");
		return 1;
	}

	sqlite3_exec(db, "pragma cache_size=-64000", NULL, NULL, NULL);
	sqlite3_exec(db, schema,                     NULL, NULL, NULL);

	sqlite3_prepare_v2(
		db,
		"insert into wl_version (timestamp,log,major,minor,patch,project,advanced) values (?,?,?,?,?,?,?)",
		-1,
		&st_version,
		NULL
	);

	sqlite3_prepare_v2(
		db,
		"insert into wl_encounter_start (timestamp,encounter,difficulty,group_size,instance,name) values (?,?,?,?,?,?)",
		-1,
		&st_encounter_start,
		NULL
	);

	sqlite3_prepare_v2(
		db,
		"insert into wl_encounter_end (timestamp,encounter,difficulty,group_size,duration,success,name) values (?,?,?,?,?,?,?)",
		-1,
		&st_encounter_end,
		NULL
	);

	sqlite3_prepare_v2(
		db,
		"insert into wl_map_change (timestamp,instance,name) values (?,?,?)",
		-1,
		&st_map_change,
		NULL
	);

	sqlite3_prepare_v2(
		db,
		"insert into wl_zone_change (timestamp,instance,difficulty,name) values (?,?,?,?)",
		-1,
		&st_zone_change,
		NULL
	);

	sqlite3_prepare_v2(
		db,
		"insert into wl_unit_died (timestamp,guid,name) values (?,?,?)",
		-1,
		&st_unit_died,
		NULL
	);

	sqlite3_prepare_v2(
		db,
		"insert into wl_spell_damage (timestamp,source_guid,source_name,target_guid,target_name,spell_id,spell_name,damage_raw,damage) values (?,?,?,?,?,?,?,?,?)",
		-1,
		&st_spell_damage,
		NULL
	);

	sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);

	const char* ptr = buf;
	while (ptr && *ptr != '\0') {
		int64_t  ts;
		wl_event e;
		
		const char* optr = ptr;
		ptr = wl_parse(&ts, &e, ptr);
		if (!ptr && wl_error) {
			printf("%.*s\n", strchr(optr, '\n') - optr, optr);
			fprintf(stderr, "%s - %.24s: failed to parse file", wl_error, wl_error_ptr);
			return 1;
		}

		insert(ts, &e);
	}

	sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);

	munmap(buf, sb.st_size);
	close(fd);

	return 0;
}
