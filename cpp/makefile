LIBAV_PATH = /Users/ephraimpark/Documents/lib/libav-12.2
ALLFFLIBS = avcodec avdevice avfilter avformat avresample avutil swscale
LDFLAGS = $(ALLFFLIBS:%=-L$(LIBAV_PATH)/lib%)

all: AVCodecContextWrapper AVFormatContextWrapper AVDecoder AudioAVFrameProcessor main

main: AVCodecContextWrapper AVFormatContextWrapper AVDecoder AudioAVFrameProcessor
		g++ -std=c++1y -Wall -c main.cpp
		g++ $(LDFLAGS) -std=c++1y -Wall -o main main.o AVCodecContextWrapper.o AVFormatContextWrapper.o AVDecoder.o AudioAVFrameProcessor.o -lavdevice -lavfilter -lavformat -lavcodec -lavresample -lswscale -lavutil -framework VideoDecodeAcceleration -framework Foundation -framework CoreVideo -lbz2 -lz

AVCodecContextWrapper: AVCodecContextWrapper.cpp AVCodecContextWrapper.h
		g++ -std=c++1y -Wall -c AVCodecContextWrapper.cpp

AVFormatContextWrapper: AVFormatContextWrapper.cpp AVFormatContextWrapper.h
		g++ -std=c++1y -Wall -c AVFormatContextWrapper.cpp

AVDecoder: AVDecoder.cpp AVDecoder.h AVCodecContextWrapper.h AVFormatContextWrapper.h AVFrameProcessor.h
		g++ -std=c++1y -Wall -c AVDecoder.cpp

AudioAVFrameProcessor: AudioAVFrameProcessor.cpp AudioAVFrameProcessor.h AVFrameProcessor.h
		g++ -std=c++1y -Wall -c AudioAVFrameProcessor.cpp

clean:
	rm AVCodecContextWrapper.o AVFormatContextWrapper.o AVDecoder.o AudioAVFrameProcessor.o main.o main
