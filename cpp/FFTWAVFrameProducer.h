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

 private:
  std::ifstream is_;
  FFTWMetaData fftwMetaData_;
  double* inBuffer_;
  double* outBuffer_;
  uint8_t* sampleBuffer_;
  fftw::DftR2RWrapper dftR2RWrapperBackward_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
