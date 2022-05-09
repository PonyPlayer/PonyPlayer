//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 10 2018
//  File : utils.cc
//******************************************

#include "include/liblrc/utils.h"

#include <regex>

namespace {
// G1: minutes, G2: seconds
static const std::regex kTimestampExp(R"exp(^(\d+):(\d\d(\.?\d+)?)$)exp");
}  // namespace

namespace lrc {

namespace utils {

int32_t TimeStringToMilliseconds(const std::string& time_str) {
  std::smatch match;
  if (!std::regex_search(time_str, match, kTimestampExp)) {
    return -1;
  }
  uint32_t minutes = std::stoi(match[1]);
  float seconds = std::stof(match[2]);
  if (seconds >= 60) {
    return -1;
  }
  return minutes * 60000 + static_cast<int32_t>(seconds * 1000);
}

std::string MillisecondsToTimeString(int32_t milliseconds) {
  uint32_t minutes = milliseconds / 60000;
  float seconds = (milliseconds % 60000) / 1000.f;
  char buffer[10];
  sprintf(buffer, "%02u:%06.3f", minutes, seconds);
  return buffer;
}

}  // namespace utils

}  // namespace lrc
