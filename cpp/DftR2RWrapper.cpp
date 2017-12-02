#include "DftR2RWrapper.h"

#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <string.h>

namespace ephiepark {
namespace fftw {

DftR2RWrapper::DftR2RWrapper(int inputSize, bool isForward): inputSize_(inputSize) {
  in_ = (double *) malloc(sizeof(double) * inputSize_);
  out_ = (double *) malloc(sizeof(double) * inputSize_);
  plan_ = fftw_plan_r2r_1d(
      inputSize_, in_, out_, isForward ? FFTW_R2HC : FFTW_HC2R, FFTW_MEASURE);
}

DftR2RWrapper::~DftR2RWrapper() {
  fftw_destroy_plan(plan_);
  free(in_);
  free(out_);
}

void DftR2RWrapper::process(double *in, double *out) {
  memcpy(in_, in, sizeof(double) * inputSize_);
  fftw_execute(plan_);
  memcpy(out, out_, sizeof(double) * inputSize_);
}

} /* namespace fftw */
} /* namespace ephiepark */
