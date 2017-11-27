#include "FFTWAVFrameProcessor.h"

#include <iostream>

namespace ephiepark {
namespace media_decode {

FFTWAVFrameProcessor::FFTWAVFrameProcessor(
  int inputSize
): inputSize_(inputSize), dftR2CWrapper_(inputSize) {
  in_ = (double *) malloc(sizeof(double) * inputSize_);
  out_ = (double *) malloc(sizeof(double) * inputSize_);
  inputCount_ = 0;
}

FFTWAVFrameProcessor::~FFTWAVFrameProcessor() {
  free(in_);
  free(out_);
}

void FFTWAVFrameProcessor::processNextAVFrame(AVFrame *avFrame) {
  for (int i = 0; i < avFrame->nb_samples; i++) {
    // Considering only one channel for now
    in_[inputCount_] = (double) *((short *) avFrame->data[0]+i);
    inputCount_++;
    if (inputCount_ == inputSize_) {
      process();
    }
  }
}

void FFTWAVFrameProcessor::process() {
  inputCount_ = 0;
  dftR2CWrapper_.process(in_, out_);
  for (int i = 0; i < inputSize_; i++) {
    std::cout << out_[i] << " ";
  }
  std::cout << std::endl;
}

} /* namespace media_decode */
} /* namespace ephiepark */
