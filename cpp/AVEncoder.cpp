#include "AVEncoder.h"

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


void AVEncoder::initWithFile(const std::string& filename) {
  if (avFormatContextWrapper_) {
    throw std::runtime_error("AVEncoder already initialized");
  }
  avFormatContextWrapper_ = std::make_unique<AVFormatContextWrapper>();
  avFormatContextWrapper_->initWithFileEncode(filename);
}

void AVEncoder::registerAVFrameProducer(
  std::unique_ptr<AVCodecContextWrapper>&& avCodecContextWrapper,
  std::unique_ptr<AVFrameProducer>&& avFrameProducer
) {
  avCodecContextWrappers_.emplace_back(std::move(avCodecContextWrapper));
  avFrameProducers_.emplace_back(std::move(avFrameProducer));
}

int AVEncoder::encodeNextFrame() {
  static AVPacket encodedAvpkt;
  static AVFrame frame;
  static auto avFrameProducerIt = avFrameProducers_.begin();

  int error;
  char errorBuffer[ERROR_BUFFER_SIZE];

  encodedAvpkt.stream_index = avFrameProducerIt - avFrameProducers_.begin();

  if ((*avFrameProducerIt)->produceNextAVFrame(&frame) < 0) {
    if (avFrameProducerIt == avFrameProducers_.end()) {
      if ((error = av_write_trailer(avFormatContextWrapper_->get())) < 0) {
        get_error_text(error, errorBuffer);
        throw std::runtime_error(errorBuffer);
      }
      return -1;
    } else {
      avFrameProducerIt++;
    }
  }
  if ((error = avcodec_send_frame(
      avCodecContextWrappers_[encodedAvpkt.stream_index]->get(),
      &frame)
    ) < 0
  ) {
    get_error_text(error, errorBuffer);
    throw std::runtime_error(errorBuffer);
  }
  while ((error = avcodec_receive_packet(
      avCodecContextWrappers_[encodedAvpkt.stream_index]->get(),
      &encodedAvpkt)
    ) == 0
  ) {
    av_write_frame(avFormatContextWrapper_->get(), &encodedAvpkt);
  }
  if (error != AVERROR(EAGAIN)) {
    get_error_text(error, errorBuffer);
    throw std::runtime_error(errorBuffer);
  }
  return 0;
}

} /* namespace media_decode */
} /* namespace ephiepark */