#include "AVDecoder.h"
#include "AudioAVFrameProcessor.h"

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

    auto processor = std::make_unique<ephiepark::media_decode::AudioAVFrameProcessor>(outfile);
    auto decoder = std::make_unique<ephiepark::media_decode::AVDecoder>();
    decoder->initWithFile(infile);
    decoder->registerAVFrameProcessor(0, std::move(processor));
    while (decoder->decodeNextFrame() == 0) {}
    return 0;
}
