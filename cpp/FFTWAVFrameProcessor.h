#pragma once

#include "AVFrameProcessor.h"

#include "DftR2CWrapper.h"

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
  double *in_;
  double *out_;
  fftw::DftR2CWrapper dftR2CWrapper_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
