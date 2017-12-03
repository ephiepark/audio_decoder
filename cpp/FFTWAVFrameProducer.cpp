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
): is_(filename, std::ifstream::binary), dftR2RWrapperBackward_(0, false) {
  is_.read((char *) &fftwMetaData_, sizeof(FFTWMetaData));
  dftR2RWrapperBackward_ = fftw::DftR2RWrapper(fftwMetaData_.windowSize, false);
  inBuffer_ = new double[fftwMetaData_.windowSize];
  outBuffer_ = new double[fftwMetaData_.windowSize];
  sampleBuffer_ = new uint8_t[fftwMetaData_.windowSize];
}

FFTWAVFrameProducer::~FFTWAVFrameProducer() {
  is_.close();
  delete inBuffer_;
  delete outBuffer_;
  delete sampleBuffer_;
}

int FFTWAVFrameProducer::produceNextAVFrame(AVFrame *avFrame) {
  static int64_t pts = 0;
  is_.read((char *) inBuffer_, sizeof(double) * fftwMetaData_.windowSize);
  dftR2RWrapperBackward_.process(inBuffer_, outBuffer_);
  for (int i = 0; i < fftwMetaData_.windowSize; i++) {
    sampleBuffer_[i] = (uint8_t) outBuffer_[i];
  }
  avFrame->data[0] = sampleBuffer_;
  avFrame->linesize[0] = sizeof(uint8_t) * fftwMetaData_.windowSize;
  avFrame->nb_samples = fftwMetaData_.windowSize;
  avFrame->format = AV_SAMPLE_FMT_U8;
  avFrame->pts = pts;
  pts = pts + fftwMetaData_.windowSize;
  avFrame->sample_rate = fftwMetaData_.sampleRate;
  avFrame->channel_layout = av_get_default_channel_layout(
    1 /* number of channel */);
  // TODO return -1 on EOF
  return 0;
}

} /* namespace media_decode */
} /* namespace ephiepark */
