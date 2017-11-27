/**
 * This module defines the interface for AVFrameProcessor
 */

#pragma once

#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif

extern "C" {
#include "libavformat/avformat.h"
};


namespace ephiepark {
namespace media_decode {

class AVFrameProcessor {
 public:
  virtual ~AVFrameProcessor() {};
  virtual void processNextAVFrame(AVFrame *) = 0;
};

} /* namespace media_decode */
} /* namespace ephiepark */
