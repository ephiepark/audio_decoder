#include "AVFormatContextWrapper.h"

extern "C" {
#include "libavutil/avstring.h"
}

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
    if (isEncode_) {
      avio_close(avFormatContext_->pb);
      avformat_free_context(avFormatContext_);
    } else {
      avformat_close_input(&avFormatContext_);
    }
  }
}

void AVFormatContextWrapper::initWithFile(const std::string& filename) {
  if (avFormatContext_) {
    throw std::runtime_error("AVFormatContextWrapper already initialized");
  }

  isEncode_ = false;

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

void AVFormatContextWrapper::initWithFileEncode(
    const std::string& filename) {
  if (avFormatContext_) {
    throw std::runtime_error("AVFormatContextWrapper already initialized");
  }

  isEncode_ = true;

  av_register_all();

  int error = 0;
  char error_buffer[ERROR_BUFFER_SIZE];

  AVIOContext *output_io_context = NULL;

  /** Open the output file to write to it. */
  if ((error = avio_open(&output_io_context, filename.c_str(),
      AVIO_FLAG_WRITE)) < 0) {
     get_error_text(error, error_buffer);
     throw std::runtime_error(error_buffer);
  }

  /** Create a new format context for the output container format. */
  if (!(avFormatContext_ = avformat_alloc_context())) {
    throw std::runtime_error("Could not allocate output format context");
  }

  /** Associate the output file (pointer) with the container format context. */
  avFormatContext_->pb = output_io_context;

  /** Guess the desired container format based on the file extension. */
  if (!(avFormatContext_->oformat = av_guess_format(NULL,
      filename.c_str(), NULL))) {
    avformat_close_input(&avFormatContext_);
    avFormatContext_ = nullptr;
    throw std::runtime_error("Could not find output file format");
  }

  av_strlcpy(avFormatContext_->filename, filename.c_str(),
      sizeof(avFormatContext_->filename));
}

::AVFormatContext* AVFormatContextWrapper::get() const {
  return avFormatContext_;
}

} /* namespace media_decode */
} /* namespace ephiepark */
