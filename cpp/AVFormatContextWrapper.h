/**
 * This module implements a thin cpp wrapper around struct AVFormatContext
 * from libav.
 */

#pragma once

#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif

extern "C" {
#include "libavformat/avformat.h"
}

#include <string>

namespace ephiepark {
namespace media_decode {

class AVFormatContextWrapper {
 public:
  AVFormatContextWrapper() {}
  ~AVFormatContextWrapper();

  /**
   * AVFormatContext should not be copied.
   * Having two instances of AFormatContext pointing to same av_format_context
   * will cause problems for deallocation, etc.
   */
  AVFormatContextWrapper(const AVFormatContextWrapper&) = delete;
  AVFormatContextWrapper& operator=(const AVFormatContextWrapper&) = delete;

  /**
   * Initialize av_format_context with the given file.
   * It will throw Exception when initialization fails.
   */
  void initWithFile(const std::string& filename);

  ::AVFormatContext* get() const;

 private:
  ::AVFormatContext* avFormatContext_ = nullptr;
};

} /* namespace media_decode */
} /* namespace ephiepark */
