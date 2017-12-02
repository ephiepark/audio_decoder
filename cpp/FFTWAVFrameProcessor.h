#pragma once

#include "AVFrameProcessor.h"

#include "DftR2RWrapper.h"

namespace ephiepark {
namespace media_decode {

class FFTWAVFrameProcessor : public AVFrameProcessor {
 public:
  FFTWAVFrameProcessor(int inputSize);
  ~FFTWAVFrameProcessor();

  virtual void processNextAVFrame(AVFrame *);

 private:
  void process();

 private:
  int inputSize_;
  int inputCount_;
  double *inForward_;
  double *outForward_;
  double *inBackward_;
  double *outBackward_;
  fftw::DftR2RWrapper dftR2RWrapperForward_;
  fftw::DftR2RWrapper dftR2RWrapperBackward_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
