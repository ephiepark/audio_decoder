#include "FFTWAVFrameProducer.h"

#include <iostream>

extern "C" {
#include "libavutil/samplefmt.h"
#include "libavutil/channel_layout.h"
}

namespace ephiepark {
namespace media_decode {

FFTWAVFrameProducer::FFTWAVFrameProducer(
  std::string filename
): is_(filename, std::ifstream::binary), dftR2RWrapperBackward_(), frameSize_(0) {
  is_.read((char *) &fftwMetaData_, sizeof(FFTWMetaData));
  dftR2RWrapperBackward_ = std::make_unique<fftw::DftR2RWrapper>(fftwMetaData_.windowSize, false);
  inBuffer_ = (double *) malloc(sizeof(double) * fftwMetaData_.windowSize);
  outBuffer_ = (double *) malloc(sizeof(double) * fftwMetaData_.windowSize);
  sampleBuffer_ = (int16_t *) malloc(sizeof(int16_t) * fftwMetaData_.windowSize);
  frameBuffer_ = nullptr;
  sampleLeft_ = 0;
  frameBufferSize_ = 0;
}

FFTWAVFrameProducer::~FFTWAVFrameProducer() {
  is_.close();
  free(inBuffer_);
  free(outBuffer_);
  free(sampleBuffer_);
  free(frameBuffer_);
}

int FFTWAVFrameProducer::fillFrameBuffer() {
  frameBufferSize_ = 0;
  while (frameBufferSize_ < frameSize_) {
    if (sampleLeft_ == 0) {
      if (is_) {
        is_.read((char *) inBuffer_, sizeof(double) * fftwMetaData_.windowSize);
        dftR2RWrapperBackward_->process(inBuffer_, outBuffer_);
        for (int i = 0; i < fftwMetaData_.windowSize; i++) {
          sampleBuffer_[i] = (int16_t) (outBuffer_[i] / fftwMetaData_.windowSize);
        }
      } else {
        for (int i = 0; i < fftwMetaData_.windowSize; i++) {
          sampleBuffer_[i] = 0;
        }
      }
      sampleLeft_ = fftwMetaData_.windowSize;
    }
    frameBuffer_[frameBufferSize_] =
        sampleBuffer_[fftwMetaData_.windowSize - sampleLeft_];
    frameBufferSize_++;
    sampleLeft_--;
  }
  if (!is_) {
    return -1;
  }
  return 0;
}

int FFTWAVFrameProducer::produceNextAVFrame(AVFrame *avFrame) {
  // TODO would this use of static variable be messed up when there are
  // multiple FFTWAVFrameProducer instances?
  static int64_t pts = 0;
  if (fillFrameBuffer() == 0) {
    avFrame->data[0] = (uint8_t *) frameBuffer_;
    avFrame->extended_data = avFrame->data;
    avFrame->linesize[0] = sizeof(int16_t) * frameSize_;
    avFrame->nb_samples = frameSize_;
    avFrame->format = AV_SAMPLE_FMT_S16;
    avFrame->pts = pts;
    avFrame->pkt_dts = avFrame->pts;
    pts = pts + frameSize_;
    avFrame->sample_rate = fftwMetaData_.sampleRate;
    avFrame->channel_layout = av_get_default_channel_layout(
      1 /* number of channel */);
    return 0;
  }
  return -1;
}

void FFTWAVFrameProducer::setFrameSize(int frameSize) {
  frameSize_ = frameSize;
  if (frameBuffer_) {
    free(frameBuffer_);
  }
  frameBuffer_ = (int16_t *) malloc(sizeof(int16_t) * frameSize_);
}

int FFTWAVFrameProducer::getSampleRate() {
  return fftwMetaData_.sampleRate;
}

} /* namespace media_decode */
} /* namespace ephiepark */
