#pragma once

namespace ephiepark {
namespace media_decode {

struct FFTWMetaData {
  // sample rate of underlying audio samples
  int sampleRate;
  // window size of samples used for fft
  int windowSize;
};

} /* namespace media_decode */
} /* namespace ephiepark */
