/**
 * This module encodes media container.
 */

#pragma once

#include "AVCodecContextWrapper.h"
#include "AVFormatContextWrapper.h"
#include "AVFrameProducer.h"

#include <memory>
#include <vector>

namespace ephiepark {
namespace media_decode {

class AVEncoder {
 public:
  AVEncoder(): avFormatContextWrapper_(nullptr),
      avCodecContextWrappers_(0), avFrameProducers_(0) {}

  void initWithFile(const std::string& filename);

  void registerAVFrameProducer(
    std::unique_ptr<AVCodecContextWrapper>&& avCodecContextWrapper,
    std::unique_ptr<AVFrameProducer>&& avFrameProducer);

  // Returns 0 on sucess < 0 on EOF
  int encodeNextFrame();

  AVFormatContext *getAVFormatContext();

 private:
  std::unique_ptr<AVFormatContextWrapper> avFormatContextWrapper_;
  std::vector<std::unique_ptr<AVCodecContextWrapper>> avCodecContextWrappers_;
  std::vector<std::unique_ptr<AVFrameProducer>> avFrameProducers_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
