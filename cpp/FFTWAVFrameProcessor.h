#pragma once

#include <fstream>

#include "AVFrameProcessor.h"

#include "DftR2RWrapper.h"
#include "Util.h"

namespace ephiepark {
namespace media_decode {

class FFTWAVFrameProcessor : public AVFrameProcessor {
 public:
  FFTWAVFrameProcessor(int inputSize, std::string filename);
  ~FFTWAVFrameProcessor();

  virtual void processNextAVFrame(AVFrame *);

 private:
  void process();

 private:
  std::ofstream os_;
  int inputSize_;
  int inputCount_;
  double *inForward_;
  double *outForward_;
  fftw::DftR2RWrapper dftR2RWrapperForward_;
};

} /* namespace media_decode */
} /* namespace ephiepark */
