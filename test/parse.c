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

UTEST(parse, event_kind) {
  const char *str = "01/01/1970 00:00:00.0000 UNIT_DIED,...";
  time_t tt;
  wl_event event;

  const wl_return_code rc = wl_parse(&tt, &event, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(event.kind, wl_unit_died);
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
