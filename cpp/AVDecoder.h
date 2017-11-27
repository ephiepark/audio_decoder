/**
 * This module decodes media container.
 */

#pragma once

#include "AVCodecContextWrapper.h"
#include "AVFormatContextWrapper.h"
#include "AVFrameProcessor.h"

#include <memory>
#include <vector>

namespace ephiepark {
namespace media_decode {

class AVDecoder {
 public:
  AVDecoder(): avFormatContextWrapper_(nullptr), avCodecContextWrappers_(0)  {}
  // ~AVDecoder();

  void initWithFile(const std::string& filename);

  void registerAVFrameProcessor(
    int streamIdx,
    std::unique_ptr<AVFrameProcessor>&& avFrameProcessor);

  // Returns 0 on sucess < 0 on EOF
  int decodeNextFrame();


 private:
  void initCodecContextWrapper(int streamIdx);

 private:
  std::unique_ptr<AVFormatContextWrapper> avFormatContextWrapper_;
  std::vector<std::unique_ptr<AVCodecContextWrapper>> avCodecContextWrappers_;
  std::vector<std::unique_ptr<AVFrameProcessor>> avFrameProcessors_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
