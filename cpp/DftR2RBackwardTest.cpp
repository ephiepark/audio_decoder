#include <iostream>

#include "AVEncoder.h"
#include "FFTWAVFrameProducer.h"
#include "AVCodecContextWrapper.h"

int main(int argc, char **argv)
{
    const char *infile;
    const char *outfile;

    if (argc < 3) {
      printf("You must specify input and output file\n");
      exit(1);
    }
    infile = argv[1];
    outfile = argv[2];

    auto avCodecContextWrapper = std::make_unique<ephiepark::media_decode::AVCodecContextWrapper>();
    auto producer = std::make_unique<ephiepark::media_decode::FFTWAVFrameProducer>(infile);
    auto encoder = std::make_unique<ephiepark::media_decode::AVEncoder>();
    encoder->initWithFile(outfile);

    avCodecContextWrapper->initEncode(
      encoder->getAVFormatContext(),
      AV_CODEC_ID_MP2,
      1 /* channels */,
      producer->getSampleRate(),
      96000 /* bit rate kbits/s */);
    encoder->registerAVFrameProducer(std::move(avCodecContextWrapper), std::move(producer));
    while (encoder->encodeNextFrame() == 0) {}
    return 0;
}
