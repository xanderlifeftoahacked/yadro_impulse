#include "io/input_parser.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace fs = std::filesystem;
using namespace io;

class InputParserTest : public ::testing::Test {
protected:
  void TearDown() override {
    if (fs::exists(test_file)) {
      fs::remove(test_file);
    }
  }

  void create_test_file(const std::string &content) {
    test_file = "test_input.txt";
    std::ofstream f(test_file);
    f << content;
    f.close();
  }

  std::string test_file;
};

TEST_F(InputParserTest, ValidInput) {
  create_test_file("3\n"
                   "09:00 19:00\n"
                   "10\n"
                   "09:15 1 client1\n"
                   "09:30 2 client1 1\n");

  auto [config, events] = InputParser::parse_input_file(test_file);

  EXPECT_EQ(config.num_tables_, 3);
  EXPECT_EQ(config.opening_time_, Time(9, 0));
  EXPECT_EQ(config.closing_time_, Time(19, 0));
  EXPECT_EQ(config.hour_cost_, 10);

  ASSERT_EQ(events.size(), 2);
  EXPECT_EQ(events[0].to_string(), "09:15 1 client1");
  EXPECT_EQ(events[1].to_string(), "09:30 2 client1 1");
}

TEST_F(InputParserTest, InvalidTableCount) {
  create_test_file("0\n"
                   "09:00 19:00\n"
                   "10\n");

  EXPECT_THROW(InputParser::parse_input_file(test_file), std::runtime_error);
}

TEST_F(InputParserTest, TimeFormatError) {
  create_test_file("3\n"
                   "09:00 25:00\n"
                   "10\n");

  try {
    InputParser::parse_input_file(test_file);
  } catch (const std::exception &e) {
    EXPECT_TRUE(std::string(e.what()).find("Line 2") != std::string::npos);
  }
}

TEST_F(InputParserTest, EventsOutOfOrder) {
  create_test_file("3\n"
                   "09:00 19:00\n"
                   "10\n"
                   "09:30 1 client1\n"
                   "09:15 1 client2\n");

  EXPECT_THROW(InputParser::parse_input_file(test_file), std::runtime_error);
}

TEST_F(InputParserTest, MaxValues) {
  create_test_file("65535\n"
                   "00:00 23:59\n"
                   "4294967295\n");

  auto [config, _] = InputParser::parse_input_file(test_file);
  EXPECT_EQ(config.num_tables_, 65535);
  EXPECT_EQ(config.hour_cost_, 4294967295);
}

TEST_F(InputParserTest, MissingFile) {
  EXPECT_THROW(InputParser::parse_input_file("non_existent.txt"),
               std::invalid_argument);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
