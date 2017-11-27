/**
 * This module implements concrete class for AudioAVFrameProcessor
 */

#pragma once

#include "AVFrameProcessor.h"

#include <fstream>

namespace ephiepark {
namespace media_decode {

class AudioAVFrameProcessor : public AVFrameProcessor {
 public:
  AudioAVFrameProcessor(const char* filename): outfile_(filename) {}
  ~AudioAVFrameProcessor();

  virtual void processNextAVFrame(AVFrame *);

 private:
  std::ofstream outfile_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
