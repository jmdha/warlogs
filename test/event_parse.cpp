#include <gtest/gtest.h>
#include <variant>
#include <warlogs/event.hpp>

TEST(EventParse, Undefined) {
  const std::string str = "0/00/0000 00:00:00.0000  "
                          "THIS_IS_NOT_A_REAL_EVENT";

  const Event::Event event = Event::Parse(str);

  EXPECT_TRUE(std::holds_alternative<Event::Undefined>(event));
  const Event::Undefined unwrapped = std::get<Event::Undefined>(event);
  EXPECT_EQ(unwrapped.event, "THIS_IS_NOT_A_REAL_EVENT");
}

TEST(EventParse, Version) {
  const std::string str = "0/00/0000 00:00:00.0000  "
                          "COMBAT_LOG_VERSION,0,ADVANCED_LOG_ENABLED,1,BUILD_"
                          "VERSION,2.3.4,PROJECT_ID,1";

  const Event::Event event = Event::Parse(str);

  EXPECT_TRUE(std::holds_alternative<Event::Version>(event));
  const Event::Version unwrapped = std::get<Event::Version>(event);
  EXPECT_TRUE(unwrapped.advanced);
  EXPECT_EQ(unwrapped.log, 0);
  EXPECT_EQ(unwrapped.major, 2);
  EXPECT_EQ(unwrapped.minor, 3);
  EXPECT_EQ(unwrapped.patch, 4);
}
