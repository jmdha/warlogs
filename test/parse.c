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
