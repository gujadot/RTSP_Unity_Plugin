#pragma once


//=============================
// Includes
//-----------------------------
// FFMPEG is writen in C so we need to use extern "C"
//-----------------------------
extern "C" {
	//#define INT64_C(x) (x ## LL)
	//#define UINT64_C(x) (x ## ULL)

#include <libavutil\opt.h>
#include <libavutil/mathematics.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>

}

