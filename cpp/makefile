LIBAV_PATH = /Users/ephraimpark/Documents/lib/libav-12.2
ALLFFLIBS = avcodec avdevice avfilter avformat avresample avutil swscale
LDFLAGS = $(ALLFFLIBS:%=-L$(LIBAV_PATH)/lib%)

all: AVCodecContextWrapper AVFormatContextWrapper AVDecoder AudioAVFrameProcessor FFTWAVFrameProcessor AudioDecodeTest DftR2CWrapper DftR2CTest

AudioDecodeTest: AudioDecodeTest.cpp AVCodecContextWrapper AVFormatContextWrapper AVDecoder AudioAVFrameProcessor
		g++ -std=c++1y -Wall -c AudioDecodeTest.cpp
		g++ $(LDFLAGS) -std=c++1y -Wall -o AudioDecodeTest AudioDecodeTest.o AVCodecContextWrapper.o AVFormatContextWrapper.o AVDecoder.o AudioAVFrameProcessor.o -lavdevice -lavfilter -lavformat -lavcodec -lavresample -lswscale -lavutil -framework VideoDecodeAcceleration -framework Foundation -framework CoreVideo -lbz2 -lz

DftR2CTest: DftR2CTest.cpp DftR2CWrapper
		g++ -std=c++1y -Wall -c DftR2CTest.cpp
		g++ $(LDFLAGS) -std=c++1y -Wall -o DftR2CTest DftR2CTest.o AVCodecContextWrapper.o AVFormatContextWrapper.o AVDecoder.o FFTWAVFrameProcessor.o DftR2CWrapper.o -lavdevice -lavfilter -lavformat -lavcodec -lavresample -lswscale -lavutil -framework VideoDecodeAcceleration -framework Foundation -framework CoreVideo -lbz2 -lz -lfftw3

AVCodecContextWrapper: AVCodecContextWrapper.cpp AVCodecContextWrapper.h
		g++ -std=c++1y -Wall -c AVCodecContextWrapper.cpp

AVFormatContextWrapper: AVFormatContextWrapper.cpp AVFormatContextWrapper.h
		g++ -std=c++1y -Wall -c AVFormatContextWrapper.cpp

AVDecoder: AVDecoder.cpp AVDecoder.h AVCodecContextWrapper.h AVFormatContextWrapper.h AVFrameProcessor.h
		g++ -std=c++1y -Wall -c AVDecoder.cpp

AudioAVFrameProcessor: AudioAVFrameProcessor.cpp AudioAVFrameProcessor.h AVFrameProcessor.h
		g++ -std=c++1y -Wall -c AudioAVFrameProcessor.cpp

FFTWAVFrameProcessor: FFTWAVFrameProcessor.cpp FFTWAVFrameProcessor.h AVFrameProcessor.h DftR2CWrapper.h
		g++ -std=c++1y -Wall -c FFTWAVFrameProcessor.cpp

DftR2CWrapper: DftR2CWrapper.cpp DftR2CWrapper.h
		g++ -std=c++1y -Wall -c DftR2CWrapper.cpp

clean:
	rm AVCodecContextWrapper.o AVFormatContextWrapper.o AVDecoder.o AudioAVFrameProcessor.o AudioDecodeTest.o AudioDecodeTest DftR2CWrapper.o DftR2CTest.o DftR2CTest FFTWAVFrameProcessor.o
