#include "AudioAVFrameProcessor.h"

namespace ephiepark {
namespace media_decode {

AudioAVFrameProcessor::~AudioAVFrameProcessor() {
  outfile_.close();
}

void AudioAVFrameProcessor::processNextAVFrame(AVFrame *avFrame) {
  for (int i = 0; i < avFrame->nb_samples; i++) {
    // Considering only one channel for now
    outfile_ << *((short *) avFrame->data[0]+i) << std::endl;
  }
}

} /* namespace media_decode */
} /* namespace ephiepark */
