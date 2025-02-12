#include "utest.h"
#include "../warlogs.h"

UTEST(parse, timestamp) {
  const char *str = "01/01/1970 00:00:00.0000 abc,...";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 0);
}

UTEST(parse, event_version) {
  const char *str = "01/01/1970 00:00:00.0000  COMBAT_LOG_VERSION,22,ADVANCED_LOG_ENABLED,1,BUILD_VERSION,11.0.7,PROJECT_ID,1";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(event.kind, wl_version);
  EXPECT_EQ(event.version.log, 22);
  EXPECT_EQ(event.version.advanced, true);
  EXPECT_EQ(event.version.major, 11);
  EXPECT_EQ(event.version.minor, 0);
  EXPECT_EQ(event.version.patch, 7);
}

UTEST(parse, encounter_start) {
  const char *str = "01/01/1970 00:00:00.0000  ENCOUNTER_START,1146,\"Randolph Moloch\",1,5,34";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(event.kind, wl_encounter_start);
  EXPECT_EQ(event.encounter_start.encounter, 1146);
  EXPECT_EQ(event.encounter_start.difficulty, 1);
  EXPECT_EQ(event.encounter_start.group_size, 5);
  EXPECT_EQ(event.encounter_start.instance, 34);
  EXPECT_STREQ(event.encounter_start.name, "Randolph Moloch");
}

UTEST(parse, encounter_end) {
  const char *str = "01/01/1970 00:00:00.0000  ENCOUNTER_END,2435,\"Sylvanas Windrunner\",15,16,1,671425";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(event.kind, wl_encounter_end);
  EXPECT_EQ(event.encounter_end.encounter, 2435);
  EXPECT_EQ(event.encounter_end.difficulty, 15);
  EXPECT_EQ(event.encounter_end.group_size, 16);
  EXPECT_EQ(event.encounter_end.success, true);
  EXPECT_EQ(event.encounter_end.time, 671425);
  EXPECT_STREQ(event.encounter_start.name, "Sylvanas Windrunner");
}

UTEST(parse, event_map_change) {
  const char *str = "01/01/1970 00:00:00.0000  MAP_CHANGE,2341,\"The Stonevault\",230.000000,-397.083008,503.437256,-437.187256";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(wl_map_change, event.kind);
  EXPECT_EQ(2341, event.map_change.id);
  EXPECT_STREQ("The Stonevault", event.map_change.name);
}

UTEST(parse, event_zone_change) {
  const char *str = "01/01/1970 00:00:00.0000  ZONE_CHANGE,2652,\"The Stonevault\",23";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(wl_zone_change, event.kind);
  EXPECT_EQ(2652, event.zone_change.instance);
  EXPECT_EQ(23, event.zone_change.difficulty);
  EXPECT_STREQ("The Stonevault", event.zone_change.name);
}

UTEST(parse, event_unit_died) {
  const char *str = "01/01/1970 00:00:00.0000  UNIT_DIED,0000000000000000,nil,0x80000000,0x80000000,Creature-0-4249-2652-10634-224516-00000A5D36,\"Skardyn Invader\",0xa48,0x0,0";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(wl_unit_died, event.kind);
  EXPECT_STREQ("Creature-0-4249-2652-10634-224516-00000A5D36", event.unit_died.id);
  EXPECT_STREQ("Skardyn Invader", event.unit_died.name);
}
