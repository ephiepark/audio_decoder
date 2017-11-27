#include "AVFormatContextWrapper.h"

#include <stdexcept>
#include <string>

namespace ephiepark {
namespace media_decode {

namespace {

static constexpr int ERROR_BUFFER_SIZE = 255;

void get_error_text(const int error, char* error_buffer)
{
  av_strerror(error, error_buffer, ERROR_BUFFER_SIZE);
}

}

AVFormatContextWrapper::~AVFormatContextWrapper() {
  if (avFormatContext_) {
    avformat_close_input(&avFormatContext_);
  }
}

void AVFormatContextWrapper::initWithFile(const std::string& filename) {
  if (avFormatContext_) {
    throw std::runtime_error("AVFormatContextWrapper already initialized");
  }

  av_register_all();

  int error = 0;
  char error_buffer[ERROR_BUFFER_SIZE];

  /** Open the input file to read from it. */
  if ((error = avformat_open_input(&avFormatContext_, filename.c_str(), NULL,
                                   NULL)) < 0) {
    avFormatContext_ = nullptr;
    get_error_text(error, error_buffer);
    throw std::runtime_error(error_buffer);
  }

  /** Get information on the input file (number of streams etc.). */
  if ((error = avformat_find_stream_info(avFormatContext_, NULL)) < 0) {
    avformat_close_input(&avFormatContext_);
    avFormatContext_ = nullptr;
    get_error_text(error, error_buffer);
    throw std::runtime_error(error_buffer);
  }
}

::AVFormatContext* AVFormatContextWrapper::get() const {
  return avFormatContext_;
}

} /* namespace media_decode */
} /* namespace ephiepark */
