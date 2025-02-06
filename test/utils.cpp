#include <gtest/gtest.h>
#include <warlogs/utils.hpp>

namespace warlogs {
TEST(Timestamp, Y2000) {
  const std::string str = "1/01/2000 00:00:00.0000";

  const std::time_t t = Timestamp(str);

  EXPECT_EQ(t, 946684800);
}

TEST(Timestamp, Mid) {
  const std::string str = "8/8/2000 8:8:8.0000";

  const std::time_t t = Timestamp(str);

  EXPECT_EQ(t, 965722088);
}

TEST(Timestamp, Specific) {
  const std::string str = "1/18/2025 14:09:01.7451";

  const std::time_t t = Timestamp(str);

  EXPECT_EQ(t, 1737209341);
}

TEST(Timestamp, Midday) {
  const std::string str = "1/18/2025 00:09:01.7451";

  const std::time_t t = Timestamp(str);

  EXPECT_EQ(t, 1737158941);
}
}
