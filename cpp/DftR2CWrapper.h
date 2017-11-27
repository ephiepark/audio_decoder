#pragma once

#include <fftw3.h>

namespace ephiepark {
namespace fftw {

class DftR2CWrapper {
 public:
  DftR2CWrapper(int inputSize);
  ~DftR2CWrapper();

  int getSize() const { return inputSize_; }
  void process(double *in, double *out);

 private:
  int inputSize_;
  double *in_;
  fftw_complex *out_;
  fftw_plan plan_;
};

} /* namespace fftw */
} /* namespace ephiepark */
