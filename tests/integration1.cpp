#include "core/club_state.hpp"
#include "core/time.hpp"
#include <gtest/gtest.h>
#include <sstream>

using namespace core;

TEST(IntegrationTest, FullScenario) {
  const std::string input = "3\n"
                            "09:00 19:00\n"
                            "10\n"
                            "08:48 1 client1\n"
                            "09:41 1 client1\n"
                            "09:48 1 client2\n"
                            "09:52 3 client1\n"
                            "09:54 2 client1 1\n"
                            "10:25 2 client2 2\n"
                            "10:58 1 client3\n"
                            "10:59 2 client3 3\n"
                            "11:30 1 client4\n"
                            "11:35 2 client4 2\n"
                            "11:45 3 client4\n"
                            "12:33 4 client1\n"
                            "12:43 4 client2\n"
                            "15:52 4 client4\n";

  const std::vector<std::string> expected_output = {
      "09:00",
      "08:48 1 client1",
      "08:48 13 NotOpenYet",
      "09:41 1 client1",
      "09:48 1 client2",
      "09:52 3 client1",
      "09:52 13 ICanWaitNoLonger!",
      "09:54 2 client1 1",
      "10:25 2 client2 2",
      "10:58 1 client3",
      "10:59 2 client3 3",
      "11:30 1 client4",
      "11:35 2 client4 2",
      "11:35 13 PlaceIsBusy",
      "11:45 3 client4",
      "12:33 4 client1",
      "12:33 12 client4 1",
      "12:43 4 client2",
      "15:52 4 client4",
      "19:00 11 client3",
      "19:00",
      "1 70 05:58",
      "2 30 02:18",
      "3 90 08:01"};

  std::istringstream iss(input);
  int num_tables;
  Time open, close;
  std::string open_time, close_time;
  int hour_cost;
  std::vector<Event> events;

  iss >> num_tables;
  iss >> open_time >> close_time;
  open = Time::from_string(open_time);
  close = Time::from_string(close_time);
  iss >> hour_cost;

  std::string line;
  while (std::getline(iss, line)) {
    if (line.empty())
      continue;
    events.push_back(Event::from_string(line));
  }

  ClubState club(num_tables, open, close, hour_cost);
  std::vector<Event> output;

  output.emplace_back(
      Event(open, EventType::CLUB_OPEN, std::vector<std::string>{}));

  for (const auto &event : events) {
    output.push_back(event);
    club.process_event(event, output);
  }

  club.close(output);
  output.emplace_back(
      Event(close, EventType::CLUB_CLOSE, std::vector<std::string>{}));

  std::vector<std::string> result;
  for (const auto &e : output) {
    result.push_back(e.to_string());
  }

  for (uint16_t i = 1; i <= num_tables; ++i) {
    const auto &t = club.table(i);
    result.push_back(
        std::to_string(i) + " " + std::to_string(t.revenue) + " " +
        Time(t.total_minutes / 60, t.total_minutes % 60).to_string());
  }

  ASSERT_EQ(result.size(), expected_output.size());
  for (size_t i = 0; i < result.size(); ++i) {
    EXPECT_EQ(result[i], expected_output[i])
        << "Mismatch at line " << i + 1 << "\nExpected: " << expected_output[i]
        << "\nActual:   " << result[i];
  }
}
