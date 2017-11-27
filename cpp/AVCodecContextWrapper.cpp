#include "AVCodecContextWrapper.h"

#include <stdexcept>

namespace ephiepark {
namespace media_decode {

namespace {

static constexpr int ERROR_BUFFER_SIZE = 255;

void get_error_text(const int error, char* error_buffer)
{
    av_strerror(error, error_buffer, ERROR_BUFFER_SIZE);
}

}

AVCodecContextWrapper::~AVCodecContextWrapper() {
  if (avCodecContext_) {
    avcodec_free_context(&avCodecContext_);
  }
}

void AVCodecContextWrapper::init(AVCodecParameters *codecpar) {
  if (avCodecContext_) {
    throw std::runtime_error("AVCodecContextWrapper already initialized");
  }

  avcodec_register_all();

  AVCodec *input_codec;

  int error = 0;
  char error_buffer[ERROR_BUFFER_SIZE];

  /** Find a decoder from AVCodecParameters. */
  if (!(input_codec = avcodec_find_decoder(codecpar->codec_id))) {
    throw std::runtime_error("Could not find input codec");
  }

  /** allocate a new decoding context */
  avCodecContext_ = avcodec_alloc_context3(input_codec);
  if (!avCodecContext_) {
    throw std::runtime_error("Could not find input codec");
  }

  /** initialize the stream parameters with demuxer information */
  error = avcodec_parameters_to_context(avCodecContext_, codecpar);
  if (error < 0) {
      avcodec_free_context(&avCodecContext_);
      get_error_text(error, error_buffer);
      throw std::runtime_error(error_buffer);
  }

  /** Open the decoder to use it later. */
  if ((error = avcodec_open2(avCodecContext_, input_codec, NULL)) < 0) {
      avcodec_free_context(&avCodecContext_);
      get_error_text(error, error_buffer);
      throw std::runtime_error(error_buffer);
  }
}

::AVCodecContext* AVCodecContextWrapper::get() const {
  return avCodecContext_;
}

} /* namespace media_decode */
} /* namespace ephiepark */
