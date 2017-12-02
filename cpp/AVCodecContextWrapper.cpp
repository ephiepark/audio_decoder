#include "AVCodecContextWrapper.h"

extern "C" {
#include "libavutil/channel_layout.h"
}

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


void AVCodecContextWrapper::initEncode(
    AVFormatContext *avFormatContext, enum AVCodecID codecId, int channels,
    int sample_rate, int bit_rate) {
  if (avCodecContext_) {
    throw std::runtime_error("AVCodecContextWrapper already initialized");
  }

  avcodec_register_all();

  AVStream *stream               = NULL;
  AVCodec *output_codec          = NULL;

  int error = 0;
  char error_buffer[ERROR_BUFFER_SIZE];


  /** Find the encoder to be used by its name. */
  if (!(output_codec = avcodec_find_encoder(codecId))) {
    throw std::runtime_error("Could not find the specified encoder");
  }

  /** Create a new audio stream in the output file container. */
  if (!(stream = avformat_new_stream(avFormatContext, NULL))) {
    throw std::runtime_error("Could not create new stream");
  }

  avCodecContext_ = avcodec_alloc_context3(output_codec);
  if (!avCodecContext_) {
    throw std::runtime_error("Could not allocate an encoding context");
  }

  /**
   * Set the basic encoder parameters.
   * The input file's sample rate is used to avoid a sample rate conversion.
   */
  avCodecContext_->channels = channels;
  avCodecContext_->channel_layout = av_get_default_channel_layout(channels);
  avCodecContext_->sample_rate = sample_rate;
  avCodecContext_->sample_fmt = output_codec->sample_fmts[0];
  avCodecContext_->bit_rate = bit_rate;

  /** Set the sample rate for the container. */
  stream->time_base.den = sample_rate;
  stream->time_base.num = 1;

  /**
   * Some container formats (like MP4) require global headers to be present
   * Mark the encoder so that it behaves accordingly.
   */
  if (avFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
    avCodecContext_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

  /** Open the encoder for the audio stream to use it later. */
  if ((error = avcodec_open2(avCodecContext_, output_codec, NULL)) < 0) {
    avcodec_free_context(&avCodecContext_);
    get_error_text(error, error_buffer);
    throw std::runtime_error(error_buffer);
  }

  error = avcodec_parameters_from_context(stream->codecpar, avCodecContext_);
  if (error < 0) {
    avcodec_free_context(&avCodecContext_);
    throw std::runtime_error("Could not initialize stream parameters");
  }
}

::AVCodecContext* AVCodecContextWrapper::get() const {
  return avCodecContext_;
}

} /* namespace media_decode */
} /* namespace ephiepark */
