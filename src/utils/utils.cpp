#include "utils/utils.hpp"
#include <algorithm>

namespace utils {

size_t count_digits(const std::string &s) {
  return std::count_if(s.begin(), s.end(),
                       [](unsigned char c) { return std::isdigit(c); });
}

} // namespace utils
