#include "AVDecoder.h"

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


void AVDecoder::initWithFile(const std::string& filename) {
  if (avFormatContextWrapper_) {
    throw std::runtime_error("AVDecoder already initialized");
  }
  avFormatContextWrapper_ = std::make_unique<AVFormatContextWrapper>();
  avFormatContextWrapper_->initWithFile(filename);
  avFrameProcessors_.resize(
    avFormatContextWrapper_->get()->nb_streams);
  avCodecContextWrappers_.resize(
    avFormatContextWrapper_->get()->nb_streams);
}

void AVDecoder::initCodecContextWrapper(int streamIdx) {
  if (!avFormatContextWrapper_) {
    throw std::runtime_error("avFormatContextWrapper_ not initialized");
  }
  if (!avCodecContextWrappers_[streamIdx]) {
    avCodecContextWrappers_[streamIdx] =
      std::make_unique<AVCodecContextWrapper>();
    avCodecContextWrappers_[streamIdx]->init(
      avFormatContextWrapper_->get()->streams[streamIdx]->codecpar);
  }
}

void AVDecoder::registerAVFrameProcessor(
  int streamIdx,
  std::unique_ptr<AVFrameProcessor>&& avFrameProcessor
) {
  avFrameProcessors_[streamIdx] = std::move(avFrameProcessor);
  initCodecContextWrapper(streamIdx);
}

int AVDecoder::decodeNextFrame() {
  static AVPacket avpkt;
  static AVFrame decodedFrame;

  int error;
  char errorBuffer[ERROR_BUFFER_SIZE];

  if ((error = av_read_frame(avFormatContextWrapper_->get(), &avpkt)) < 0) {
    // Currently, assuming that there won't be an error other than EOF
    return error;
  }
  if (avFrameProcessors_[avpkt.stream_index]) {
    if ((error = avcodec_send_packet(
        avCodecContextWrappers_[avpkt.stream_index]->get(),
        &avpkt)
      ) < 0
    ) {
      get_error_text(error, errorBuffer);
      throw std::runtime_error(errorBuffer);
    }
    while ((error = avcodec_receive_frame(
        avCodecContextWrappers_[avpkt.stream_index]->get(),
        &decodedFrame)
      ) == 0
    ) {
      avFrameProcessors_[avpkt.stream_index]->processNextAVFrame(&decodedFrame);
    }
    if (error != AVERROR(EAGAIN)) {
      get_error_text(error, errorBuffer);
      throw std::runtime_error(errorBuffer);
    }
  }
  return 0;
}

} /* namespace media_decode */
} /* namespace ephiepark */
