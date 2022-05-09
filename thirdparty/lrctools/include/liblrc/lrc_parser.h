//******************************************
//  Author : Yuwei Huang
//  Created On : Sat Feb 10 2018
//  File : lrc_parser.h
//******************************************

#ifndef LIBLRC_LRC_PARSER_H
#define LIBLRC_LRC_PARSER_H

#include <istream>
#include <memory>

#include "lyrics.h"

namespace lrc {

class LrcParser {
 public:
  LrcParser();
  ~LrcParser();

  std::unique_ptr<Lyrics> ParseStream(std::istream* stream) const;

  std::unique_ptr<Lyrics> ParseString(const std::string& lrc_string) const;

  // Returns nullptr if there is trouble loading the file.
  std::unique_ptr<Lyrics> ParseFile(const std::string& filename) const;

 private:
  LrcParser(const LrcParser&) = delete;
  LrcParser(LrcParser&&) = delete;
};

}  // namespace lrc

#endif  // LIBLRC_LRC_PARSER_H
