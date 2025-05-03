#include "core/event.hpp"
#include "core/time.hpp"
#include <gtest/gtest.h>

using namespace core;

TEST(EventTest, FromStringValidInput) {
  Event event = Event::from_string("08:48 1 client1");

  EXPECT_EQ(event.get_time(), Time(8, 48));
  EXPECT_EQ(event.get_type(), EventType::CLIENT_ARRIVED);
  ASSERT_EQ(event.get_body().size(), 1);
  EXPECT_EQ(event.get_body()[0], "client1");
}

TEST(EventTest, FromStringInvalidFormat) {
  EXPECT_THROW(Event::from_string("08:48"), std::invalid_argument);
  EXPECT_THROW(Event::from_string("08:48 client1"), std::invalid_argument);
  EXPECT_THROW(Event::from_string("invalid_time 1 client1"),
               std::invalid_argument);
}

TEST(EventTest, CreateErrorEvents) {
  Event e1 = Event::create_error(Time(9, 0), ErrorType::NOT_OPEN_YET);
  EXPECT_EQ(e1.to_string(), "09:00 13 NotOpenYet");

  Event e2 = Event::create_error(Time(10, 30), ErrorType::PLACE_IS_BUSY);
  EXPECT_EQ(e2.to_string(), "10:30 13 PlaceIsBusy");
}

TEST(EventTest, ToStringFormatting) {
  Event event(Time(9, 5), EventType::CLIENT_SEATED, {"client42", "3"});
  EXPECT_EQ(event.to_string(), "09:05 2 client42 3");

  Event error(Time(23, 59), EventType::ERROR_OCCURRED, {"ClientUnknown"});
  EXPECT_EQ(error.to_string(), "23:59 13 ClientUnknown");
}

TEST(EventTest, ValidityChecks) {
  Event valid_arrival(Time(), EventType::CLIENT_ARRIVED, {"a1"});
  EXPECT_TRUE(valid_arrival.is_valid());

  Event valid_seated(Time(), EventType::CLIENT_SEATED, {"b2", "5"});
  EXPECT_TRUE(valid_seated.is_valid());

  Event invalid_arrival(Time(), EventType::CLIENT_ARRIVED, {});
  EXPECT_FALSE(invalid_arrival.is_valid());

  Event invalid_seated1(Time(), EventType::CLIENT_SEATED, {"x"});
  Event invalid_seated2(Time(), EventType::CLIENT_SEATED, {"y", "z"});
  EXPECT_FALSE(invalid_seated1.is_valid());
  EXPECT_FALSE(invalid_seated2.is_valid());

  Event invalid_error(Time(), EventType::ERROR_OCCURRED, {"UnknownError"});
  EXPECT_FALSE(invalid_error.is_valid());
}

TEST(EventTest, TimeComparison) {
  Event e1(Time(9, 0), EventType::CLIENT_ARRIVED, {"c1"});
  Event e2(Time(9, 1), EventType::CLIENT_ARRIVED, {"c2"});
  Event e3(Time(9, 0), EventType::CLIENT_LEFT, {"c1"});

  EXPECT_TRUE(e1 < e2);
  EXPECT_FALSE(e2 < e1);
  EXPECT_FALSE(e1 < e3);
}

TEST(EventTest, EdgeCases) {
  EXPECT_NO_THROW(Event::from_string("00:00 4 client"));

  Event max_id_event(Time(), EventType::ERROR_OCCURRED, {"X"});
  EXPECT_EQ(static_cast<int>(max_id_event.get_type()), 13);

  Event empty_error(Time(), EventType::ERROR_OCCURRED, {});
  EXPECT_FALSE(empty_error.is_valid());
}
