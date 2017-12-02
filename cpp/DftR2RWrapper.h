#pragma once

#include <fftw3.h>

namespace ephiepark {
namespace fftw {

class DftR2RWrapper {
 public:
  DftR2RWrapper(int inputSize, bool isForward);
  ~DftR2RWrapper();

  int getSize() const { return inputSize_; }
  void process(double *in, double *out);

 private:
  int inputSize_;
  double *in_;
  double *out_;
  fftw_plan plan_;
};

} /* namespace fftw */
} /* namespace ephiepark */
