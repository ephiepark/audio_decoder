#pragma once

#include <fstream>

#include "AVFrameProducer.h"

#include "DftR2RWrapper.h"
#include "Util.h"

namespace ephiepark {
namespace media_decode {

class FFTWAVFrameProducer : public AVFrameProducer {
 public:
  FFTWAVFrameProducer(std::string filename);
  ~FFTWAVFrameProducer();

  virtual int produceNextAVFrame(AVFrame *);
  virtual void setFrameSize(int frameSize);

  int getSampleRate();

 private:
  int fillFrameBuffer();

 private:
  std::ifstream is_;
  FFTWMetaData fftwMetaData_;
  double* inBuffer_;
  double* outBuffer_;
  int16_t* sampleBuffer_;
  int16_t* frameBuffer_;
  int sampleLeft_;
  int frameBufferSize_;
  int frameSize_;
  std::unique_ptr<fftw::DftR2RWrapper> dftR2RWrapperBackward_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
