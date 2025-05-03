#include "core/club_state.hpp"
#include "core/time.hpp"
#include <gtest/gtest.h>

using namespace core;

class ClubStateTest : public ::testing::Test {
protected:
  void SetUp() override {
    state = std::make_unique<ClubState>(3, Time::from_string("09:00"),
                                        Time::from_string("21:00"), 1000);
  }

  std::unique_ptr<ClubState> state;
  std::vector<Event> output;
};

TEST_F(ClubStateTest, Initialization) {
  EXPECT_EQ(state->num_tables_, 3);
  EXPECT_FALSE(state->tables_[1].is_occupied);
  EXPECT_FALSE(state->tables_[2].is_occupied);
  EXPECT_FALSE(state->tables_[3].is_occupied);
}

TEST_F(ClubStateTest, ClientArrival) {
  Event event1 = Event::from_string("10:00 1 client1");
  state->process_event(event1, output);
  EXPECT_EQ(state->clients_.size(), 1);

  Event event2 = Event::from_string("08:30 1 client2");
  state->process_event(event2, output);
  ASSERT_FALSE(output.empty());
  EXPECT_EQ(output.back().get_body()[0], "NotOpenYet");

  Event event3 = Event::from_string("10:05 1 client1");
  state->process_event(event3, output);
  ASSERT_FALSE(output.empty());
  EXPECT_EQ(output.back().get_body()[0], "YouShallNotPass");
}

TEST_F(ClubStateTest, ClientSeating) {
  state->process_event(Event::from_string("10:00 1 client1"), output);

  Event event1 = Event::from_string("10:05 2 client2 1");
  state->process_event(event1, output);
  ASSERT_FALSE(output.empty());
  EXPECT_EQ(output.back().get_body()[0], "ClientUnknown");

  Event event2 = Event::from_string("10:10 2 client1 1");
  state->process_event(event2, output);
  EXPECT_TRUE(state->tables_[1].is_occupied);
  EXPECT_EQ(state->clients_["client1"].table_id, 1);
}

TEST_F(ClubStateTest, ClientLeaving) {
  state->process_event(Event::from_string("10:00 1 client1"), output);
  state->process_event(Event::from_string("10:05 2 client1 1"), output);

  Event event = Event::from_string("12:00 4 client1");
  state->process_event(event, output);
  EXPECT_FALSE(state->tables_[1].is_occupied);
  EXPECT_EQ(state->clients_.size(), 0);
}

TEST_F(ClubStateTest, ClosingTime) {
  state->process_event(Event::from_string("20:30 1 client1"), output);
  state->process_event(Event::from_string("20:35 2 client1 1"), output);

  state->close(output);

  EXPECT_TRUE(output.back().get_type() == EventType::CLIENT_FORCE_LEFT);
  EXPECT_EQ(state->tables_[1].revenue, 1000);
}

TEST_F(ClubStateTest, RevenueCalculation) {
  state->seat_client("client1", 1, Time::from_string("10:00"));
  state->calculate_revenue(state->tables_[1], Time::from_string("10:15"));

  EXPECT_EQ(state->tables_[1].revenue, 1000);
  EXPECT_EQ(state->tables_[1].total_minutes, 15);
}
