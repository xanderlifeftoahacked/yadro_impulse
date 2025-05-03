#include "core/time.hpp"
#include <gtest/gtest.h>

using namespace core;
TEST(TimeTest, ConstructorValidation) {
  EXPECT_NO_THROW(Time(0, 0));
  EXPECT_NO_THROW(Time(23, 59));

  EXPECT_THROW(Time(-1, 0), std::invalid_argument);
  EXPECT_THROW(Time(24, 0), std::invalid_argument);

  EXPECT_THROW(Time(0, -1), std::invalid_argument);
  EXPECT_THROW(Time(0, 60), std::invalid_argument);
}

TEST(TimeTest, FromStringParsing) {
  EXPECT_EQ(Time::from_string("00:00"), Time(0, 0));
  EXPECT_EQ(Time::from_string("23:59"), Time(23, 59));
  EXPECT_EQ(Time::from_string("09:05"), Time(9, 5));

  EXPECT_THROW(Time::from_string("2359"), std::invalid_argument);
  EXPECT_THROW(Time::from_string("23:5"), std::invalid_argument);
  EXPECT_THROW(Time::from_string("2a:3b"), std::invalid_argument);
}

TEST(TimeTest, ToStringFormatting) {
  EXPECT_EQ(Time(9, 5).to_string(), "09:05");
  EXPECT_EQ(Time(23, 59).to_string(), "23:59");
  EXPECT_EQ(Time(0, 0).to_string(), "00:00");
}

TEST(TimeTest, ComparisonOperators) {
  Time t1(10, 30);
  Time t2(9, 45);
  Time t3(10, 30);

  EXPECT_TRUE(t2 < t1);
  EXPECT_TRUE(t1 > t2);
  EXPECT_TRUE(t1 == t3);
  EXPECT_TRUE(t1 <= t3);
  EXPECT_TRUE(t1 >= t3);
  EXPECT_TRUE(t1 != t2);
}

TEST(TimeTest, AddMinutes) {
  Time t1(10, 0);
  EXPECT_EQ(t1.add_minutes(90), Time(11, 30));
  EXPECT_EQ(t1.add_minutes(150), Time(12, 30));

  Time t2(23, 30);
  EXPECT_EQ(t2.add_minutes(30), Time(0, 0));
  EXPECT_EQ(t2.add_minutes(-60), Time(22, 30));
}

TEST(TimeTest, TimeDifference) {
  Time t1(10, 30);
  Time t2(9, 45);
  EXPECT_EQ(t1 - t2, 45);

  Time t3(0, 0);
  Time t4(23, 30);
  EXPECT_EQ(t3 - t4, 30);
}

TEST(TimeTest, EdgeCases) {
  Time t1(23, 59);
  EXPECT_EQ(t1.add_minutes(1), Time(0, 0));

  Time t2(0, 0);
  EXPECT_EQ(t2.add_minutes(-1), Time(23, 59));
}
