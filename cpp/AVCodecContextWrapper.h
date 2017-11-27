/**
 * This module implements a thin cpp wrapper around struct AVCodecContext
 * from libav.
 */

#pragma once

#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif

extern "C" {
#include "libavcodec/avcodec.h"
}


namespace ephiepark {
namespace media_decode {

class AVCodecContextWrapper {
 public:
  AVCodecContextWrapper() {}
  ~AVCodecContextWrapper();

  /**
   * AVCodecContextWrapper should not be copied.
   * Having two instances of AVCodecContext pointing to same av_codec_context
   * will cause problems for deallocation, etc.
   */
  AVCodecContextWrapper(const AVCodecContextWrapper&) = delete;
  AVCodecContextWrapper& operator=(const AVCodecContextWrapper&) = delete;

  /**
   * Initialize av_codec_context with the .
   * It will throw Exception when initialization fails.
   */
  void init(AVCodecParameters *codecpar);

  ::AVCodecContext* get() const;

 private:
  ::AVCodecContext* avCodecContext_ = nullptr;
};

} /* namespace media_decode */
} /* namespace ephiepark */
