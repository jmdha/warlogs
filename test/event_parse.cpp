#include <gtest/gtest.h>
#include <variant>
#include <warlogs/event.hpp>

namespace warlogs {
TEST(EventParse, Undefined) {
  const std::string str = "0/00/0000 00:00:00.0000  "
                          "THIS_IS_NOT_A_REAL_EVENT";

  const std::pair<std::time_t, Event> tevent = Parse(str);
  const std::time_t &timestamp = tevent.first;
  const Event& event = tevent.second;

  EXPECT_TRUE(std::holds_alternative<Undefined>(event));
  const Undefined unwrapped = std::get<Undefined>(event);
  EXPECT_EQ(unwrapped.event, "THIS_IS_NOT_A_REAL_EVENT");
}

TEST(EventParse, Version) {
  const std::string str = "0/00/0000 00:00:00.0000  "
                          "COMBAT_LOG_VERSION,1,ADVANCED_LOG_ENABLED,1,BUILD_"
                          "VERSION,2.3.4,PROJECT_ID,1";

  const std::pair<std::time_t, Event> tevent = Parse(str);
  const std::time_t &timestamp = tevent.first;
  const Event& event = tevent.second;

  EXPECT_TRUE(std::holds_alternative<Version>(event));
  const Version unwrapped = std::get<Version>(event);
  EXPECT_TRUE(unwrapped.advanced);
  EXPECT_EQ(unwrapped.log, 1);
  EXPECT_EQ(unwrapped.major, 2);
  EXPECT_EQ(unwrapped.minor, 3);
  EXPECT_EQ(unwrapped.patch, 4);
}

TEST(EventParse, ZoneChange) {
  const std::string str = "0/00/0000 00:00:00.0000  "
                          "ZONE_CHANGE,2652,\"The Stonevault\",23";

  const std::pair<std::time_t, Event> tevent = Parse(str);
  const std::time_t &timestamp = tevent.first;
  const Event& event = tevent.second;

  EXPECT_TRUE(std::holds_alternative<ZoneChange>(event));
  const ZoneChange unwrapped = std::get<ZoneChange>(event);
  EXPECT_EQ(unwrapped.instance, 2652);
  EXPECT_EQ(unwrapped.difficulty, 23);
  EXPECT_EQ(unwrapped.name, "The Stonevault");
}

TEST(EventParse, UnitDied) {
  const std::string str = "0/00/0000 00:00:00.0000  "
                          "UNIT_DIED,0000000000000000,nil,0x80000000,0x80000000,SOME_ID,\"ABC\",0x512,0x0,1";

  const std::pair<std::time_t, Event> tevent = Parse(str);
  const std::time_t &timestamp = tevent.first;
  const Event& event = tevent.second;

  EXPECT_TRUE(std::holds_alternative<UnitDied>(event));
  const UnitDied unwrapped = std::get<UnitDied>(event);
  EXPECT_EQ(unwrapped.id, "SOME_ID");
  EXPECT_EQ(unwrapped.name, "ABC");
}
}
