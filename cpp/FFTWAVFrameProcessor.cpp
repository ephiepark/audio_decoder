#include "FFTWAVFrameProcessor.h"

#include <iostream>

namespace ephiepark {
namespace media_decode {

FFTWAVFrameProcessor::FFTWAVFrameProcessor(
  int inputSize
): inputSize_(inputSize), dftR2RWrapperForward_(inputSize, true),
    dftR2RWrapperBackward_(inputSize, false) {
  inForward_ = (double *) malloc(sizeof(double) * inputSize_);
  outForward_ = (double *) malloc(sizeof(double) * inputSize_);
  inBackward_ = (double *) malloc(sizeof(double) * inputSize_);
  outBackward_ = (double *) malloc(sizeof(double) * inputSize_);
  inputCount_ = 0;
}

FFTWAVFrameProcessor::~FFTWAVFrameProcessor() {
  free(inForward_);
  free(outForward_);
  free(inBackward_);
  free(outBackward_);
}

void FFTWAVFrameProcessor::processNextAVFrame(AVFrame *avFrame) {
  for (int i = 0; i < avFrame->nb_samples; i++) {
    // Considering only one channel for now
    inForward_[inputCount_] = (double) *((short *) avFrame->data[0]+i);
    inputCount_++;
    if (inputCount_ == inputSize_) {
      process();
    }
  }
}

void FFTWAVFrameProcessor::process() {
  inputCount_ = 0;
  dftR2RWrapperForward_.process(inForward_, outForward_);

  /*
  inBackward_[0] = outForward_[0];
  for (int i = 1; i < inputSize_ / 2; i++) {
    inBackward_[i] = sqrt(outForward_[i] * outForward_[i] + outForward_[inputSize_ - i] * outForward_[inputSize_ - i]);
  }
  for (int i = inputSize_ / 2; i < inputSize_; i++) {
    inBackward_[i] = 0;
  }
  dftR2RWrapperBackward_.process(inBackward_, outBackward_);

  for (int i = 0; i < inputSize_ / 2; i++) {
    std::cout << inForward_[i] << " " << outBackward_[i] / inputSize_ << std::endl;
  }
  std::cout << std::endl;
  */
  for (int i = 1; i < inputSize_ / 2; i++) {
    std::cout << outForward_[i] << " " << outForward_[inputSize_ - i] << std::endl;
  }

}

} /* namespace media_decode */
} /* namespace ephiepark */
