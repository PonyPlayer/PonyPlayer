//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 10 2018
//  File : lrc_parser.cc
//******************************************

#include "include/liblrc/lrc_parser.h"

#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "include/liblrc/utils.h"

namespace lrc {

namespace {

// G1: metadata key, G2: metadata value
static const std::regex kMetadataExp(R"exp(\[([a-zA-Z]+):([^\]]+)\])exp");

// G1: lyric
static const std::regex kLyricExp(R"exp(\]([^\]]*)\r?$)exp");

// G1: mm, G2: ss
static const std::regex kTimestampExp(R"exp(\[(\d\d:\d\d\.\d\d)\])exp");

void SetMetadata(Lyrics::Metadata* metadata,
                 const std::string& key,
                 const std::string& val) {
  if (key == "ar")
    metadata->artist = val;
  else if (key == "al")
    metadata->album = val;
  else if (key == "ti")
    metadata->title = val;
  else if (key == "au")
    metadata->lyricsWriter = val;
  else if (key == "by")
    metadata->fileCreator = val;
  else if (key == "offset") {
    metadata->adjustment = std::stoi(val);
  } else {
    // Unknown key. Do nothing.
  }
}

}  // namespace

LrcParser::LrcParser() {}
LrcParser::~LrcParser() {}

std::unique_ptr<Lyrics> LrcParser::ParseStream(
    std::istream* stream) const {
  Lyrics::Metadata metadata {"", "", "", "", "", 0};
  std::vector<Lyrics::LyricLine> lyric_lines;

  for (std::string line; std::getline(*stream, line); ) {
    std::smatch metadata_match;
    // Assume one metadata tag per line.
    if (std::regex_search(line, metadata_match, kMetadataExp)) {
      std::string key = metadata_match[1];
      std::string val = metadata_match[2];
      SetMetadata(&metadata, key, val);
      continue;
    }

    std::smatch lyric_match;
    if (!std::regex_search(line, lyric_match, kLyricExp)) {
      // No lyric. The line is malformed.
      continue;
    }
    std::string lyric = lyric_match[1];

    // Load timestamps and push lyric lines.
    std::string::const_iterator search_start(line.cbegin());
    std::smatch timestamp_match;
    while (regex_search(
        search_start, line.cend(), timestamp_match, kTimestampExp)) {
      int32_t time_ms = utils::TimeStringToMilliseconds(timestamp_match[1]);

      Lyrics::LyricLine lyric_line;
      lyric_line.start_time = time_ms;
      lyric_line.lyric = lyric;
      lyric_lines.push_back(lyric_line);

      search_start += timestamp_match.position() + timestamp_match.length();
    }
  }
  return std::unique_ptr<Lyrics>(new Lyrics(metadata, std::move(lyric_lines)));
}

std::unique_ptr<Lyrics> LrcParser::ParseString(
    const std::string& lrc_string) const {
  std::stringstream stream(lrc_string);
  return ParseStream(&stream);
}

std::unique_ptr<Lyrics> LrcParser::ParseFile(
    const std::string& filename) const {
  std::ifstream stream(filename);
  if (!stream.is_open()) {
    return nullptr;
  }
  std::unique_ptr<Lyrics> lyrics = ParseStream(&stream);
  stream.close();
  return lyrics;
}
}  // namespace lrc
