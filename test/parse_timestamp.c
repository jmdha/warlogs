#include "utest.h"
#include "../warlogs.h"

UTEST(parse_timestamp, utc) {
  const char *str = "01/01/1970 00:00:00.0000";
  time_t tt;

  const wl_return_code rc = wl_parse_timestamp(&tt, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 0);
}

UTEST(parse_timestamp, year) {
  const char *str = "01/01/1971 00:00:00.0000";
  time_t tt;

  const wl_return_code rc = wl_parse_timestamp(&tt, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 31622400);
}

UTEST(parse_timestamp, month) {
  const char *str = "02/01/1970 00:00:00.0000";
  time_t tt;

  const wl_return_code rc = wl_parse_timestamp(&tt, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 2678400);
}

UTEST(parse_timestamp, day) {
  const char *str = "01/02/1970 00:00:00.0000";
  time_t tt;

  const wl_return_code rc = wl_parse_timestamp(&tt, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 86400);
}

UTEST(parse_timestamp, hour) {
  const char *str = "01/01/1970 01:00:00.0000";
  time_t tt;

  const wl_return_code rc = wl_parse_timestamp(&tt, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 3600);
}

UTEST(parse_timestamp, minute) {
  const char *str = "01/01/1970 00:01:00.0000";
  time_t tt;

  const wl_return_code rc = wl_parse_timestamp(&tt, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 60);
}

UTEST(parse_timestamp, sec) {
  const char *str = "01/01/1970 00:01:00.0000";
  time_t tt;

  const wl_return_code rc = wl_parse_timestamp(&tt, str);

  EXPECT_EQ(rc, wl_ok);
  EXPECT_EQ(tt, 60);
}
