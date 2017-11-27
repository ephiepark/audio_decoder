#include "DftR2CWrapper.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

namespace ephiepark {
namespace fftw {

namespace {

static constexpr int REAL = 0;
static constexpr int IMAG = 1;

double getAmplitude(double *complex_number) {
  return sqrt(
    (complex_number[REAL] * complex_number[REAL]) +
    (complex_number[IMAG] * complex_number[IMAG]));
}

}

DftR2CWrapper::DftR2CWrapper(int inputSize): inputSize_(inputSize) {
  in_ = (double *) malloc(sizeof(double) * inputSize_);
  out_ = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * inputSize_);
  plan_ = fftw_plan_dft_r2c_1d(inputSize_, in_, out_, FFTW_MEASURE);
}

DftR2CWrapper::~DftR2CWrapper() {
  fftw_destroy_plan(plan_);
  free(in_);
  fftw_free(out_);
}

void DftR2CWrapper::process(double *in, double *out) {
  memcpy(in_, in, sizeof(double) * inputSize_);
  fftw_execute(plan_);
  for (int i = 0; i < inputSize_; i++) {
    out[i] = getAmplitude(out_[i]);
  }
}

} /* namespace fftw */
} /* namespace ephiepark */
