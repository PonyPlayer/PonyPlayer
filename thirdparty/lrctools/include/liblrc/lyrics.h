//******************************************
//  Author : Yuwei Huang
//  Created On : Thu Feb 08 2018
//  File : lyrics.h
//******************************************

#ifndef LIBLRC_LYRICS_H
#define LIBLRC_LYRICS_H

#include <cstdint>
#include <string>
#include <vector>

namespace lrc {

class Lyrics {
 public:
  struct Metadata {
    std::string artist;
    std::string album;
    std::string title;
    std::string lyricsWriter;
    std::string fileCreator;
    int32_t adjustment;  // in milliseconds, + shifts time up, - shifts down.
  };

  static const int32_t kEndTimeNever;

  struct LyricLine {
    int32_t start_time;  // in milliseconds.

    // kEndTimeNever if it is at the end of the song.
    int32_t end_time;

    std::string lyric;
  };

  using ConstLyricIterator = std::vector<LyricLine>::const_iterator;

  // Lyric duration is not required and will be automatically filled.
  // Please DO NOT include offset adjustment in LyricLine. They will be adjusted
  // by this class.
  Lyrics(const Metadata& metadata, std::vector<LyricLine>&& lyrics);
  ~Lyrics();

  const Metadata& GetMetadata() const;

  // Returns the LyricLine at the given offset. Returns a null LyricLine (all
  // fields are empty) if offset < 0 or offset >= kEndTimeNever.
  // Random position lookup cost is O(log(n)); lookup with linearly increasing
  // offset has a cost of O(1) for each lookup.
  const LyricLine& LyricAt(int32_t offset);

  // Returns the lyric iterator at the given offset. Caller can use it to query
  // lyrics before or after the current lyric. Returns IteratorEnd if offset < 0
  // or offset >= kEndTimeNever.
  ConstLyricIterator LyricIteratorAt(int32_t offset);

  ConstLyricIterator IteratorBegin() const;
  ConstLyricIterator IteratorEnd() const;

 private:
  Metadata metadata_;
  std::vector<LyricLine> lyrics_;
  std::vector<LyricLine>::iterator current_lyric_;
};

}  // namespace lrc

#endif  // LIBLRC_LYRICS_H
