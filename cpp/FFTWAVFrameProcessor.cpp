#include "FFTWAVFrameProcessor.h"

#include <iostream>

namespace ephiepark {
namespace media_decode {

FFTWAVFrameProcessor::FFTWAVFrameProcessor(
  int inputSize, std::string filename
): inputSize_(inputSize), os_(filename, std::ofstream::binary), dftR2RWrapperForward_(inputSize, true) {
  inForward_ = (double *) malloc(sizeof(double) * inputSize_);
  outForward_ = (double *) malloc(sizeof(double) * inputSize_);
  inputCount_ = 0;
}

FFTWAVFrameProcessor::~FFTWAVFrameProcessor() {
  free(inForward_);
  free(outForward_);
  os_.close();
}

void FFTWAVFrameProcessor::processNextAVFrame(AVFrame *avFrame) {
  static bool isFirst = true;
  if (isFirst) {
    isFirst = false;
    FFTWMetaData fftwMetaData;
    fftwMetaData.sampleRate = avFrame->sample_rate;
    fftwMetaData.windowSize = inputSize_;
    os_.write((char *)&fftwMetaData, sizeof(FFTWMetaData));
  }
  for (int i = 0; i < avFrame->nb_samples; i++) {
    // Considering only one channel for now
    inForward_[inputCount_] = (double) *((short *) avFrame->data[0]+i);
    inputCount_++;
    if (inputCount_ == inputSize_) {
      process();
    }
  }
}

// TODO somehow need to be called one more at the end
void FFTWAVFrameProcessor::process() {
  inputCount_ = 0;
  dftR2RWrapperForward_.process(inForward_, outForward_);
  os_.write((char *) outForward_, sizeof(double) * inputSize_);
}

} /* namespace media_decode */
} /* namespace ephiepark */
