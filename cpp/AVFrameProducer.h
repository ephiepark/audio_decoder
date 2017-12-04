/**
 * This module defines the interface for AVFrameProducer
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

class AVFrameProducer {
 public:
  virtual ~AVFrameProducer() {};

  // Returns 0 on sucess < 0 on EOF
  virtual int produceNextAVFrame(AVFrame *) = 0;
  virtual void setFrameSize(int frameSize) = 0;
};

} /* namespace media_decode */
} /* namespace ephiepark */
